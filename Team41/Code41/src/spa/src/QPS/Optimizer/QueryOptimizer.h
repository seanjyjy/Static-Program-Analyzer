#pragma once

#include "QPS/QueryObject.h"
#include "QPS/Optimizer/PKBAdapter.h"
#include "QPS/SuperClause.h"
#include "ClauseDepGraph.h"
#include "OptimizedQueryObject.h"
#include "ClauseGroups.h"
#include "QueryOptimizerBuilder.h"
#include "AbstractGroups.h"
#include "ClauseGroup.h"
#include "ClauseGroups.h"
#include "SortedGroups.h"
#include "FifoGroups.h"

using namespace std;

// TODO: destructor

class QueryOptimizer {
 private:
  // configuration options
  bool isClauseGroupingEnabled = false;
  bool isIntraGroupSortEnabled = false;
  bool isInterGroupSortEnabled = false;
  bool isDynamicPollingEnabled = false;

  bool isPkbAdapterSet = false;

  PKBAdapter adapter;
  ClauseDepGraph clauseDepGraph;
  OptimizedQueryObject optimizedQueryObject;
  QueryOptimizer();

 public:
  friend class QueryOptimizerBuilder;
  friend ostream& operator<<(ostream &os, const QueryOptimizer& obj);
  static QueryOptimizerBuilder create();
  OptimizedQueryObject optimize(QueryObject &qo);

  OptimizedQueryObject getOptimizedQueryObject();
  void printPlan();
};
