/*
 * This file converts a gssw_graph to json and enables loading from the json
 */
#ifndef __GSSW_TO_JSON_HH__
#define __GSSW_TO_JSON_HH__

#include "gssw.h"
#include "nlohmann/json.hpp"

//using namespace nlohmann;
using nJson = nlohmann::json;

/*
 * @param gssw_graph* the graph to serialize
 * @return nlohman::json the json for that graph
 */
nJson dump_graph(gssw_graph* g);
/*
 * @param gssw_node* the node to serialize
 * @return nlohman::json the json for that node
 * It's a little difficult to handle dumping nodes because of the pointers to
 * other nodes. These pointers are stored as the literal long long pointer, and
 * the node has an added field, ptr, which corresponds to the memory location of
 * this node. These are used later in reconstruction
 */
nJson dump_node(gssw_node* n);
//appears this is allways null
//nJson dump_gssw_align(gssw_align* alignment);

/*
 * loads a graph from the json representation of that graph
 * @param nlhomann::json, the serialization of the graph
 * @returns gssw_graph, a new graph dynamically allocated constructed from json
 */
gssw_graph* ld_graph(nJson o);

/*
 * Dynamically constructs a gssw_node from json for that node. It initializes
 * the ptrs to null because at this point we don't have information of how the
 * pointers map to other nodes
 * @param nlophmann::json the json for the node
 * @returns gssw_node, the constructed node
 */
gssw_node* ld_node_no_edges(nJson o);

/*
 * initializes the edges of nodes according the pointers stored in the nodeArr.
 * Changes are made inplace to the nodes. As a postcondition, all edges have
 * been properly initialized
 * @param gssw_node** nodes, list of nodes to make initialize
 * @param size_t len, the length of the list
 * @param nJson the json array of the nodes. This is used to decide which edges
 *   are present for each node.
 * @Precondition: nodes must be in the same order as the nodes in nodeArr
 */
void init_node_edges(gssw_node** nodes, size_t len, nJson nodeArr);

/* constructs a mapping between old serialized pointers to new pointers to real
 * nodes. Helper method
 * @param gssw_node** nodes
 * @param gssw_node** nodes, list of nodes to make initialize
 * @param size_t len, the length of the list
 * @param nJson the json array of the nodes. This is used to decide which edges
 *   are present for each node.
 * @return map<node_ptr, node_ptr> mapping, this provides a lookup from the old
 *   serialized ptrs to the new nodes
 * @Precondition: nodes must be in the same order as the nodes in nodeArr
 */

std::map<long long,gssw_node*>* get_node_mapping(gssw_node** nodes,
                                                            size_t len, 
                                                            nJson nodeArr);
/*
 * Private helper. Used for arrays of primatives, that is types that can be
 * represented in json natively.
 */
template<typename T>
nJson dump_prim_arr(T* arr, size_t size){
  nJson jArr = nJson::array();
  for (int i = 0; i < size; i++){
    jArr.push_back(arr[i]);
  }
  return jArr;
}
/*
 * Private helper. the inverse of dump_prim. Used to load from json, arrays of
 * primatives. Used for arrays of primatives, that is types that can be
 * represented in json natively.
 */
template<typename T>
T* ld_prim_arr(nJson primArr, size_t size){
  T* arr = new T[size];
  for (int i = 0; i < size; i++){
    arr[i] = (T) primArr[i];
  }
  return arr;
}

#endif // __GSSW_TO_JSON_HH__
