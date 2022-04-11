#include "catch.hpp"
#include "QPS/Optimizer/SimpleGraph.h"
#include "TestOptimizerUtils.h"

using namespace std;

TEST_CASE("Simple Graph: general graph") {
    SimpleGraph<string> g = TestOptimizerUtils::genStringGraph(1, 6, {{1, 2},
                                                                      {1, 4},
                                                                      {2, 4},
                                                                      {3, 5}});
    vector<vector<string>> cmpts = g.getDisjointComponents();
    sort(cmpts.begin(), cmpts.end(), [](auto &a, auto &b) { return a.size() < b.size(); });
    REQUIRE(cmpts.size() == 3);
    REQUIRE(cmpts[0].size() == 1);
    REQUIRE(cmpts[1].size() == 2);
    REQUIRE(cmpts[2].size() == 3);
}

TEST_CASE("Simple Graph: sparse graph") {
    SimpleGraph<string> g = TestOptimizerUtils::genStringGraph(1, 8, {{1, 5},
                                                                      {3, 4}});
    vector<vector<string>> cmpts = g.getDisjointComponents();
    sort(cmpts.begin(), cmpts.end(), [](auto &a, auto &b) { return a.size() < b.size(); });
    REQUIRE(cmpts.size() == 6);
    REQUIRE(cmpts[0].size() == 1);
    REQUIRE(cmpts[1].size() == 1);
    REQUIRE(cmpts[2].size() == 1);
    REQUIRE(cmpts[3].size() == 1);
    REQUIRE(cmpts[4].size() == 2);
    REQUIRE(cmpts[5].size() == 2);
}

TEST_CASE("Simple Graph: complete graph") {
    vector<pair<int,int>> edges;
    for (int u = 1; u <= 8; u++) {
        for (int v = u+1; v <= 8; v++) {
            edges.emplace_back(u,v);
        }
    }
    SimpleGraph<string> g = TestOptimizerUtils::genStringGraph(1, 8, edges);
    vector<vector<string>> cmpts = g.getDisjointComponents();
    REQUIRE(cmpts.size() == 1);
    REQUIRE(cmpts[0].size() == 8);
}

TEST_CASE("Simple Graph: tree") {
    SimpleGraph<string> g = TestOptimizerUtils::genStringGraph(1, 7, {{1, 2},
                                                              {1, 3},
                                                              {2, 4},
                                                              {2, 5}});
    vector<vector<string>> cmpts = g.getDisjointComponents();
    sort(cmpts.begin(), cmpts.end(), [](auto &a, auto &b) { return a.size() < b.size(); });
    REQUIRE(cmpts.size() == 3);
    REQUIRE(cmpts[0].size() == 1);
    REQUIRE(cmpts[1].size() == 1);
    REQUIRE(cmpts[2].size() == 5);
}

TEST_CASE("Simple Graph: should disregard duplicate nodes") {
    SimpleGraph<string> g;
    vector<string> vertices = {"1", "1", "2", "3", "4", "2", "2"};
    vector<pair<string,string>> edges = {{"1","2"}, {"1","3"}};
    for (string &s: vertices) g.addVertex(s);
    for (auto &[u, v]: edges) g.addUndirectedEdge(u, v);
    vector<vector<string>> cmpts = g.getDisjointComponents();
    sort(cmpts.begin(), cmpts.end(), [](auto &a, auto &b) { return a.size() < b.size(); });
    REQUIRE(cmpts.size() == 2);
    REQUIRE(cmpts[0].size() == 1);
    REQUIRE(cmpts[1].size() == 3);
}

TEST_CASE("Simple Graph: should disregard duplicate edges") {
    SimpleGraph<string> g;
    vector<string> vertices = {"1", "2", "3", "4", "5", "6"};
    vector<pair<string,string>> edges = {{"1","2"},{"1","4"},{"2","4"},{"3","5"},
                                         {"1","4"},{"2","4"},{"2","4"}};
    for (string &s: vertices) g.addVertex(s);
    for (auto &[u, v]: edges) g.addUndirectedEdge(u, v);
    vector<vector<string>> cmpts = g.getDisjointComponents();
    sort(cmpts.begin(), cmpts.end(), [](auto &a, auto &b) { return a.size() < b.size(); });
    REQUIRE(cmpts.size() == 3);
    REQUIRE(cmpts[0].size() == 1);
    REQUIRE(cmpts[1].size() == 2);
    REQUIRE(cmpts[2].size() == 3);
}