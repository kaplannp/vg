/*
 * This file converts a gssw_graph to json and enables loading from the json
 */
#ifndef __GSSW_TO_JSON_HH__
#define __GSSW_TO_JSON_HH__

#include "gssw.h"
#include "nlohmann/json.hpp"

//using namespace nlohmann;
using nJson = nlohmann::json;

nJson dump_graph(gssw_graph* g);
nJson dump_node(gssw_node* n);
nJson dump_gssw_align(gssw_align* alignment);

#endif // __GSSW_TO_JSON_HH__
