#pragma once

#include "PKB/PKBManager.h"
#include "QPS/ClauseVariable.h"

// TODO: destructor

class PKBAdapter {
 private:
  PKBManager *pkb = nullptr;
 public:
  PKBAdapter();
  PKBAdapter(PKBManager *pkb);
  long long getRowCount(const QueryDeclaration &synonym);
  long long getRowCount(const vector<QueryDeclaration> &synonyms);
};
