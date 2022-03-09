#pragma once

#include "QPS/QueryObject.h"
#include "ClauseDepGraph.h"
#include "OptimizedQueryObject.h"
#include "ClauseGroups.h"

using namespace std;

class QueryOptimizer {
private:
    ClauseDepGraph clauseDepGraph;
    ClauseGroups divideClausesIntoGroups(QueryObject &qo);
public:
    QueryOptimizer();
    OptimizedQueryObject optimize(QueryObject &qo);
};
