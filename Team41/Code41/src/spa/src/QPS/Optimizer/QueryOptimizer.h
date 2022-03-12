#pragma once

#include "QPS/QueryObject.h"
#include "QPS/Optimizer/PKBAdapter.h"
#include "ClauseDepGraph.h"
#include "OptimizedQueryObject.h"
#include "ClauseGroups.h"

using namespace std;

class QueryOptimizer {
private:
    PKBAdapter adapter;
    ClauseDepGraph clauseDepGraph;
    ClauseGroups divideClausesIntoGroups(QueryObject &qo);
public:
    QueryOptimizer(const PKBManager& pkbManager);
    OptimizedQueryObject optimize(QueryObject &qo, bool isDynamic=false);
};
