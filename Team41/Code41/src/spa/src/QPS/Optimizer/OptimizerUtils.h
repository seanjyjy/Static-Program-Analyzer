#pragma once

#include "QPS/SuperClause.h"
#include "QPS/Selectable.h"

class OptimizerUtils {
public:
    static bool hasSynonymOverlap(const vector<QueryDeclaration> &qds, const vector<SuperClause*> &clauses);
    static vector<SuperClause *> removeDuplicates(const vector<SuperClause *> &clauses);
    static void print(vector<vector<SuperClause*>> &clauses);
};
