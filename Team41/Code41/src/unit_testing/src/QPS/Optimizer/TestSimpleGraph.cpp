#include "catch.hpp"
#include "QPS/Optimizer/SimpleGraph.h"
#include "TestOptimizerUtils.h"

using namespace std;

/*
            6
    ┌────2
    │    │
    1    │   3
    │    │   │
    │    │   │
    4────┘   │
             │
         5───┘
 */
TEST_CASE("Simple Graph") {
    SimpleGraph g = TestOptimizerUtils::genGraph(1, 6, {{1,2},{1,4},{2,4},{3,5}});
    vector<vector<string>> cmpts = g.getDisjointComponents();
    sort(cmpts.begin(), cmpts.end(), [](auto &a, auto &b) { return a.size() < b.size(); });
    REQUIRE(cmpts.size() == 3);
    REQUIRE(cmpts[0].size() == 1);
    REQUIRE(cmpts[1].size() == 2);
    REQUIRE(cmpts[2].size() == 3);
}