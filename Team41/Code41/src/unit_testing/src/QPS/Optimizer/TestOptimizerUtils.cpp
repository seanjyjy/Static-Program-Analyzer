#include "TestOptimizerUtils.h"

#include <iostream>

void TestOptimizerUtils::print(vector<vector<string>> &arr) {
    for (auto &a: arr) {
        for (auto &b: a) {
            cout << b << " ";
        }
        cout << endl;
    }
}

SimpleGraph<string> TestOptimizerUtils::genStringGraph(int fromInclusive, int toInclusive, const vector<pair<int, int>> &edges) {
    SimpleGraph<string> g;
    // add nodes
    for (int i = fromInclusive; i <= toInclusive; i++) {
        g.addVertex(to_string(i));
    }
    // add edges
    for (auto &[u, v]: edges) {
        g.addUndirectedEdge(to_string(u), to_string(v));
    }
    return g;
}

void TestOptimizerUtils::print(vector<vector<SuperClause *>> &clauses) {
    for (auto &arr: clauses) {
        cout << "array" << endl;
        for (auto &cl: arr) {
            cout << cl->toString() << " ";
        }
        cout << endl;
    }
}
