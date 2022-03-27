#pragma once

#include "QPS/QueryObject.h"
#include "QPS/Optimizer/PKBAdapter.h"
#include "QPS/SuperClause.h"
#include "ClauseDepGraph.h"
#include "OptimizedQueryObject.h"
#include "OptimizerUtils.h"
#include "AbstractGroups.h"
#include "SortedGroups.h"
#include "FifoGroups.h"
#include "SortedGroup.h"
#include "FifoGroup.h"
#include "PKBAwareGroup.h"

using namespace std;

class QueryOptimizer {
private:
    // default configuration options
    bool isClauseGroupingEnabled = true;
    bool isIntraGroupSortEnabled = true;
    bool isInterGroupSortEnabled = true;
    bool isDynamicPollingEnabled = false;
    bool isPkbAdapterSet = false;
    PKBAdapter adapter;
    ClauseDepGraph clauseDepGraph;

    QueryOptimizer();
public:

    static QueryOptimizer create();

    // convenience method to turn on all forms of optimization
    QueryOptimizer &enableAllOptimizations(PKBManager *pkbManager);

    QueryOptimizer &setIntraGroupSort(bool isOn);

    QueryOptimizer &setInterGroupSort(bool isOn);

    QueryOptimizer &setClauseGrouping(bool isOn);

    QueryOptimizer &enableDynamicPolling(PKBManager *pkbManager);

    OptimizedQueryObject optimize(QueryObject *qo);

    void print();
};
