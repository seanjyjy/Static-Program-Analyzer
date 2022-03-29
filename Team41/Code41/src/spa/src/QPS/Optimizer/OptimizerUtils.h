#pragma once

#include "QPS/SuperClause.h"
#include "QPS/Selectable.h"

class OptimizerUtils {
public:
    static vector<string> getSelectablesAsStrings(const vector<Selectable> &selectables);
    static vector<SuperClause *> removeDuplicates(const vector<SuperClause *> &clauses);
    static void print(vector<vector<SuperClause*>> &clauses);
};
