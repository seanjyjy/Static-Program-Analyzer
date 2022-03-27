#pragma once

#include "PKB/PKBManager.h"
#include "QPS/ClauseVariable.h"

class PKBAdapter {
private:
    PKBManager *pkb;
public:
    PKBAdapter();

    PKBAdapter(PKBManager *pkbMgr);

    long long getRowCount(const QueryDeclaration &synonym);

    long long getRowCount(const vector<QueryDeclaration> &synonyms);
};
