#pragma once

#include "PKB/PKBManager.h"
#include "QPS/ClauseVariable.h"

class PKBAdapter {
private:
    PKBManager pkb;
public:
    PKBAdapter(PKBManager pkb);
    long long getRowCount(const ClauseVariable &synonym);
    long long getRowCount(const vector<ClauseVariable> &synonyms);
};
