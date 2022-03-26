#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "QPS/QueryClause.h"
#include "QPS/PatternClause.h"
#include "QPS/Optimizer/SimpleGraph.h"
#include "QPS/SuperClause.h"

using namespace std;

class ClauseDepGraph {
private:
    // used to identify the group of clauses with no synonyms
    const string NO_SYNONYM = "_NO_SYNONYMS";
    SimpleGraph<string> graph;
    unordered_map<string, vector<SuperClause *>> synonymToClauses;
public:
    ClauseDepGraph();

    void registerClause(SuperClause *cl);

    vector<vector<SuperClause *>> split();
};
