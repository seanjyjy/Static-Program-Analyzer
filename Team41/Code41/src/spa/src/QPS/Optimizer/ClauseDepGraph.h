#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "QPS/QueryClause.h"
#include "QPS/PatternClause.h"
#include "QPS/Optimizer/ClauseGroups.h"
#include "QPS/Optimizer/SimpleGraph.h"
#include "QPS/SuperClause.h"

using namespace std;

// TODO desstructor
class ClauseDepGraph {
 private:
  // used to identify the group of clauses with no synonyms
  const string NO_SYNONYM = "NO_SYNONYMS";

  SimpleGraph graph;
  PKBAdapter pkbAdapter;
  unordered_map<string, vector<SuperClause *>> synonymToClauses;
  bool hasSyn(const string &s);
  vector<SuperClause *> getClausesOfSyn(const string &syn);
 public:
  ClauseDepGraph(PKBAdapter pkbAdapter);

  void registerClause(SuperClause *cl);
  ClauseGroups split();
};
