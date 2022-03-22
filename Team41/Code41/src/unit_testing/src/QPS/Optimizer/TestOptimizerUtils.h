#pragma once

#include <string>
#include <vector>
#include "QPS/Optimizer/SimpleGraph.h"
#include "QPS/SuperClause.h"

using namespace std;

class TestOptimizerUtils {
public:
    static void print(vector<vector<string>> &arr);
    static SimpleGraph genGraph(int fromInclusive, int toInclusive, const vector<pair<int, int>> &edges);
    static void print(vector<vector<SuperClause*>> &clauses);
};
