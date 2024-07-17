#include "gssw.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <cassert>

#include "gssw_to_json.hpp"

using nJson = nlohmann::json;

nJson dump_graph_mapping(gssw_graph_mapping* ggm){
  nJson o;
  o["position"] = ggm->position;
  o["score"] = ggm->score;
  //ignoring this for now. Maybe not necessary
  //o["cigar"] = call function for this
  return o;
}

nJson dump_graph(gssw_graph* g){
   nJson o;
   o["size"] = g->size;
   assert(g->max_node == nullptr); //expermintally verified. Things will break if
   //this doesn't hold true. Would need to call dump node instead. As long as
   //max_node is in node_list, it'll be fine, but otherwise you need to
   //initialize these edges somewhere when you load
   o["max_node"] = nullptr; // dump_node(g->max_node);
   o["nodes"] = nJson::array();
   for (int i = 0; i < g->size; i++){
     o["nodes"].push_back(dump_node(g->nodes[i]));
   }
   return o;
}
nJson dump_node(gssw_node* n){
  nJson o;
  o["ptr"] = (long long) n;
  if (n != nullptr){
    assert(n->data == nullptr); //experiments show this is always null. If it 
      //isn't we'll need a new function here
    o["data"] = nullptr;
    o["id"] = n->id;//std::to_string(n->id);
    o["seq"] = dump_prim_arr<char>(n->seq, n->len);
    o["num"] = dump_prim_arr<int8_t>(n->num, n->len);
    o["len"] = n->len; // length of sequence
    o["prev"] = nJson::array();
    for(int i = 0; i < n->count_prev; i++){
      o["prev"].push_back((long long) n->prev[i]);
    }
    o["count_prev"] = n->count_prev;
    o["next"] = nJson::array();
    for(int i = 0; i < n->count_next; i++){
      o["next"].push_back((long long) n->next[i]);
    }
    o["count_next"] = n->count_next;
    assert(n->alignment == nullptr); //experiments show this is always null. If 
    //it ever isn't, we'll need to add a serialization method for the alignment 
    //object
    o["alignment"] = nullptr; //dump_gssw_align(n->alignment);
  }
  return o;
}

gssw_graph* ld_graph(nJson o){
  gssw_graph* g = new gssw_graph;
  g->size = o["size"];
  g->max_node = nullptr; //o["max_node"]; //always null. No need to init edges
  g->nodes = new gssw_node*[(size_t) o["size"]];
  for(int i = 0; i < g->size; i++){
    g->nodes[i] = ld_node_no_edges(o["nodes"][i]);
  }
  //at this point nodes are initialized, but edges aren't filled in
  init_node_edges(g->nodes, g->size, o["nodes"]);
  return g;
}

gssw_node* ld_node_no_edges(nJson o){
  gssw_node* n = new gssw_node;
  n->data = nullptr; //we must change this if the result is ever not null
  n->id = o["id"];
  n->seq = new char[(size_t) o["len"]];
  for (int i = 0; i < o["len"]; i++){
    n->seq[i] = ((char) (int) o["seq"][i]); //these got dumped to shorts
  }
  n->num = ld_prim_arr<int8_t>(o["num"],o["len"]);
  n->len = o["len"]; // length of sequence
  n->prev = nullptr; //we will pass only the pointer here
  n->count_prev = o["count_prev"];
  n->next = nullptr;
  n->count_next = o["count_next"];
  n->alignment = nullptr; //o["alignment"]; //always null, don't need parse
  return n;
}

void init_node_edges(gssw_node** nodes, size_t len, nJson nodeArr){
  std::map<long long, gssw_node*>* mapping = 
    get_node_mapping(nodes, len, nodeArr);
  for (int i = 0; i < len; i++){
    gssw_node* n = nodes[i];
    nJson o = nodeArr[i];
    n->prev = new gssw_node*[(size_t) o["count_prev"]];
    for (int j = 0; j < o["count_prev"]; j++){
      n->prev[j] = mapping->at((long long) o["prev"][j]);
    }
    n->next = new gssw_node*[(size_t) o["count_next"]];
    for (int j = 0; j < o["count_next"]; j++){
      n->next[j] = mapping->at((long long) o["next"][j]);
    }
  }
  delete mapping; //If it recursively deletes it's Luke's fault
}

std::map<long long,gssw_node*>* get_node_mapping(gssw_node** nodes,
                                                            size_t len, 
                                                            nJson nodeArr){
  std::map<long long, gssw_node*>* mapping = 
        new std::map<long long, gssw_node*>();
  for (int i = 0; i < len; i++){
    (*mapping)[(long long) nodeArr[i]["ptr"]] = nodes[i];
  }
  return mapping;
}

//This never is not null, it must be initialized later, so we're gonna leave
//it off for now
//nJson dump_gssw_align(gssw_align* alignment){
//  nJson o;
//  if (alignment != nullptr){
//    std::cerr << "zkn! it's not null!" << std::endl;
//  }
//  return o;
//}
