#pragma once

#include "QPS/QueryObject.h"
#include "QPS/Optimizer/PKBAdapter.h"
#include "QPS/SuperClause.h"
#include "ClauseDepGraph.h"
#include "OptimizedQueryObject.h"
#include "ClauseGroups.h"
//#include "QueryOptimizerBuilder.h"
#include "AbstractGroups.h"
#include "ClauseGroup.h"
#include "ClauseGroups.h"
#include "SortedGroups.h"
#include "FifoGroups.h"
#include "SortedGroup.h"
#include "FifoGroup.h"
#include "PKBAwareGroup.h"

using namespace std;

// TODO: destructor

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

    static vector<SuperClause *> removeDuplicates(const vector<SuperClause *> &clauses);

public:

    static QueryOptimizer create();

    QueryOptimizer &setIntraGroupSort(bool isOn);

    QueryOptimizer &setInterGroupSort(bool isOn);

    QueryOptimizer &setClauseGrouping(bool isOn);

    QueryOptimizer &enableDynamicPolling(const PKBAdapter &pkbAdapter);

    OptimizedQueryObject optimize(QueryObject *qo);

    void print();
};
