#pragma once

#include <vector>

using namespace std;

#include "QPS/SuperClause.h"
#include "QPS/Optimizer/PKBAdapter.h"
#include "QPS/Optimizer/TableEstimate.h"

// TODO maybe subclass clause groups into static/dynamic?
class ClauseGroup {
 private:
  vector<SuperClause *> clauses;
  vector<bool> isUsed;
  TableEstimate table;
 public:
  ClauseGroup();
  ClauseGroup(PKBAdapter pkbAdapter);
  void addClause(SuperClause *c);
  void sortClauses();
  bool hasNextClause();
  SuperClause *getNextClauseStatic(); // reads clauses in the same order they were added
  SuperClause *getNextClauseDynamic(); // gets the clause resulting in the smallest intermediate table
  long getScore();
};
