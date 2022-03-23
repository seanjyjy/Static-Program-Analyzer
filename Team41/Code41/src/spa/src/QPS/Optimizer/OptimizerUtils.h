#pragma once

#include "QPS/SuperClause.h"

class OptimizerUtils {
public:
    static vector<SuperClause *> removeDuplicates(const vector<SuperClause *> &clauses);
    static void print(vector<vector<SuperClause*>> &clauses);
};
