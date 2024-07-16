#include "gssw.h"
#include "nlohmann/json.hpp"

#include "gssw_to_json.hpp"

using nJson = nlohmann::json;

nJson dump_graph(gssw_graph* g){
   nJson o;
   o["size"] = g->size;
   o["max_node"] = dump_node(g->max_node);
   o["nodes"] = nJson::array();
   for (int i = 0; i < g->size; i++){
     o["nodes"].push_back(dump_node(g->nodes[i]));
   }
   return o;
}
nJson dump_node(gssw_node* n){
  nJson o;
  //o["data"] = nullptr; //TODO
  o["id"] = n->id;
  char* seq; // sequence
  for (int i = 0; i < 
  //int8_t* num; // numerical conversion of seq
  o["len"] = n->len; // length of sequence
  //gssw_node** prev;
  o["count_prev"] = n->count_prev;
  //gssw_node** next;
  o["count_next"] = n->count_next;
  //gssw_align* alignment;
  return o;
}


nJson dump_gssw_align(gssw_align* alignment){
  nJson o;
  return o;
}
