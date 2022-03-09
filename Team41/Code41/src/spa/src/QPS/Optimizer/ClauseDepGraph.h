#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "QPS/QueryClause.h"
#include "QPS/PatternClause.h"
#include "QPS/Optimizer/ClauseGroups.h"
#include "QPS/Optimizer/SimpleGraph.h"
#include "TempClause.h"

using namespace std;

class ClauseDepGraph {
private:
    // used to identify the group of clauses with no synonyms
    const static string NO_SYNONYM = "NO_SYNONYMS";

    SimpleGraph graph;
    unordered_map<string, vector<TempClause>> synonymToClauses;
    bool hasSyn(const string&s);
    vector<TempClause> getClausesOfSyn(const string& syn);
public:
    void registerClause(TempClause tc);
    ClauseGroups split();
};
