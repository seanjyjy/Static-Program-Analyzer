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
    using TaggedSuperClause = pair<SuperClause*, int>;

    int cid = 0;
    const string NO_SYNONYM = "_NO_SYNONYMS"; // used to identify the group of clauses with no synonyms
    SimpleGraph<string> graph;
    unordered_map<string, vector<TaggedSuperClause>> synonymToClauses; // maps synonyms to clauses with that synonym
    unordered_map<int, bool> isCidProcessed; // stop clauses from being processed multiple times

    // convenience method to get the next cid and update it
    int getCid();
    // convenience method to retrieve only unprocessed clauses
    vector<SuperClause*> getUnprocessedClauses(vector<TaggedSuperClause> &clauses);
public:
    ClauseDepGraph();

    void registerClause(SuperClause *cl);

    vector<vector<SuperClause *>> split();
};
