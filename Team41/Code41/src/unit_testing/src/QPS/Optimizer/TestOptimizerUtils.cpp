#include "catch.hpp"
#include "TestOptimizerUtils.h"
#include "QPS/Optimizer/OptimizedQueryObject.h"
#include "QPS/QueryObject.h"
#include "QPS/Optimizer/QueryOptimizer.h"
#include "QPS/QueryParser.h"

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

void TestOptimizerUtils::ensureOQOIsCorrect(string &query, PKBManager &pkbManager) {
    // parse the query and setup objects
    QueryParser qp = QueryParser{query};
    QueryObject *qo(qp.parse());
    OptimizedQueryObject oqo = QueryOptimizer::create()
            .enableAllOptimizations(&pkbManager)
            .optimize(qo);
    oqo.printPlan();

    // extract clauses from original
    vector<SuperClause*> originalClauses(qo->getSuperClauses());
    unordered_set<SuperClause> originalSet;
    for (SuperClause *cl: originalClauses) {
        originalSet.insert(*cl);
    }

    // extract clauses from optimized
    vector<SuperClause*> optimizedClauses;
    unordered_set<SuperClause> optimizedSet;
    while (!oqo.empty()) {
        SuperClause *cl = oqo.popClause();
        optimizedClauses.push_back(cl);
        optimizedSet.insert(*cl);
    }

    // query optimizer should not duplicate clauses
    REQUIRE(originalClauses.size() <= optimizedClauses.size());

    // query optimizer should not add or remove new clauses
    REQUIRE(originalSet.size() == optimizedSet.size());

    // TODO delete causing bugs in some places, ignore memory leaks for now
//    delete qo;
}