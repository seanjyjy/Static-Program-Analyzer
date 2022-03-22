#pragma once

#include "ClauseGroups.h"
#include "QPS/QueryObject.h"
#include "QPS/Optimizer/TableEstimate.h"
#include "QPS/Optimizer/PKBAdapter.h"
#include "QPS/SuperClause.h"
#include "QPS/Selectable.h"

// TODO: explore inheritance instead of composition
class OptimizedQueryObject : public QueryObject {
 private:
  ClauseGroups clauseGroups;
  ClauseGroup currSource;

  // for dynamic optimization
  bool isDynamic = false;
 public:
  OptimizedQueryObject();
  OptimizedQueryObject(PKBAdapter &pkb, const QueryObject &qo, ClauseGroups &cg, bool isDynamic = false);
  SuperClause *nextClause();
  bool hasNextClause();
};
