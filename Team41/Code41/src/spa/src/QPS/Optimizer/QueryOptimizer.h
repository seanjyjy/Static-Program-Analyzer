#pragma once

#include "QPS/QueryObject.h"
#include "QPS/Optimizer/PKBAdapter.h"
#include "QPS/SuperClause.h"
#include "ClauseDepGraph.h"
#include "OptimizedQueryObject.h"
#include "ClauseGroups.h"

using namespace std;

// TODO: destructor

class QueryOptimizer {
private:
    PKBAdapter adapter;
    ClauseDepGraph clauseDepGraph;
    OptimizedQueryObject optimizedQueryObject;
    ClauseGroups divideClausesIntoGroups(QueryObject &qo);
public:
    QueryOptimizer(PKBManager *pkbManager);
    void optimize(QueryObject &qo, bool isDynamic=false);
    OptimizedQueryObject getOptimizedQueryObject();
    void printPlan();
};
