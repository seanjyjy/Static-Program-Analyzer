#include "catch.hpp"
#include "TestOptimizerUtils.h"
#include "QPS/Optimizer/OptimizedQueryObject.h"
#include "QPS/QueryObject.h"
#include "QPS/Optimizer/QueryOptimizer.h"
#include "QPS/QueryParser.h"
#include "QPS/Optimizer/SortedGroups.h"

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
    int id = 0;
    for (auto &arr: clauses) {
        cout << "group " + to_string(id++) + " ";
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

    // extract clauses from original
    vector<SuperClause*> originalClauses(qo->getSuperClauses());
    unordered_set<SuperClause*, PointedSuperClauseHash, PointedSuperClauseEq> originalSet;
    for (SuperClause *cl: originalClauses) {
        originalSet.insert(cl);
    }

    // extract clauses from optimized
    vector<SuperClause*> optimizedClauses;
    unordered_set<SuperClause*, PointedSuperClauseHash, PointedSuperClauseEq> optimizedSet;
    while (!oqo.empty()) {
        SuperClause *cl = oqo.popClause();
        optimizedClauses.push_back(cl);
        optimizedSet.insert(cl);
    }

    // query optimizer should not duplicate clauses
    REQUIRE(originalClauses.size() >= optimizedClauses.size());

    // query optimizer should not add or remove new clauses
    REQUIRE(originalSet.size() == optimizedSet.size());

    delete qo;
}

void TestOptimizerUtils::print(vector<QueryDeclaration> arr) {
    cout << "query declarations: ";
    for (QueryDeclaration& qd: arr) {
        cout << qd.toString() << " ";
    }
    cout << endl;
}

vector<vector<SuperClause *>> TestOptimizerUtils::prepareClauseGroups(string &s) {
    ClauseDepGraph g;
    QueryParser qp = QueryParser{s};
    unique_ptr<QueryObject> queryObject(qp.parse());
    for (SuperClause *cl: queryObject->getSuperClauses()) {
        g.registerClause(cl);
    }
    queryObject->cleanUp();
    return g.split();
}

void TestOptimizerUtils::testGroupandGroupsAbstractions(string &query) {
    ClauseDepGraph g;
    QueryParser qp = QueryParser{query};
    unique_ptr<QueryObject> queryObject(qp.parse());
    vector<SuperClause*> superClauses(queryObject->getSuperClauses());
    for (SuperClause *cl: superClauses) g.registerClause(cl);

    vector<ClauseGroup*> fifoGroups;
    vector<ClauseGroup*> sortedGroups;
    vector<ClauseGroup*> pkbAwareGroups;

    // empty pkb manager
    PKBManager pkbManager = PKBManager();
    PKBAdapter pkbAdapter(&pkbManager);

    vector<vector<SuperClause*>> clauseGroups = g.split();
    size_t totalClauses = 0;
    for (vector<SuperClause*> &clauseGroup: clauseGroups) {
        auto *fg = new FifoGroup(clauseGroup);
        auto *sg = new SortedGroup(clauseGroup);
        auto *pg = new PKBAwareGroup(clauseGroup, pkbAdapter);

        // should be same size as the base clause group
        REQUIRE(fg->size() == clauseGroup.size());
        REQUIRE(sg->size() == clauseGroup.size());
        REQUIRE(pg->size() == clauseGroup.size());

        for (size_t i = 0; i < clauseGroup.size(); i++) {
            // this will only run if size is greater than 0, no underflow possible
            if (i == clauseGroup.size()-1) {
                REQUIRE(fg->isLast());
                REQUIRE(sg->isLast());
                REQUIRE(pg->isLast());
            }
            // should not be empty -> should not give errors when popping
            REQUIRE_NOTHROW(fg->pop());
            REQUIRE_NOTHROW(sg->pop());
            REQUIRE_NOTHROW(pg->pop());
            totalClauses++;
        }

        // all clauses should be consumed
        REQUIRE(fg->empty());
        REQUIRE(sg->empty());
        REQUIRE(pg->empty());

        delete fg;
        delete sg;
        delete pg;

        // for later
        fifoGroups.push_back(new FifoGroup(clauseGroup));
        sortedGroups.push_back(new SortedGroup(clauseGroup));
        pkbAwareGroups.push_back(new PKBAwareGroup(clauseGroup, pkbAdapter));
    }

    // the groups should not add or remove clauses
    REQUIRE(totalClauses == superClauses.size());

    // individual clause group OK, now test clause groups abstraction
    FifoGroups fg(fifoGroups);
    SortedGroups sg(sortedGroups);
    SortedGroups pg(pkbAwareGroups);

    // manually sort the groups to emulate the actual consumption
    sort(sortedGroups.begin(), sortedGroups.end(), [](ClauseGroup *a, ClauseGroup *b) {
        return ClauseGroupComparator()(a, b);
    });
    int clauseGroupsSize = (int) clauseGroups.size();
    for (int i = 0; i < clauseGroupsSize; i++) {
        size_t groupSize = clauseGroups[i].size();
        size_t sortedGroupSize = sortedGroups[i]->size();
        REQUIRE(fg.currGroupSize() == groupSize);
        REQUIRE(sg.currGroupSize() == sortedGroupSize);
        REQUIRE(pg.currGroupSize() == sortedGroupSize);

        int size = (int) groupSize;
        for (int j = 0; j < size; j++) {
            // this will only run if size is greater than 0, no underflow possible
            if (j == size-1) {
                REQUIRE(fg.isLastOfGroup());
            }
            REQUIRE_NOTHROW(fg.pop());
        }

        int sortedSize = (int) sortedGroupSize;
        for (int j = 0; j < sortedSize; j++) {
            // this will only run if size is greater than 0, no underflow possible
            if (j == sortedSize-1) {
                REQUIRE(sg.isLastOfGroup());
                REQUIRE(pg.isLastOfGroup());
            }
            REQUIRE_NOTHROW(sg.pop());
            REQUIRE_NOTHROW(pg.pop());
        }
    }

    REQUIRE(fg.empty());
    REQUIRE(sg.empty());
    REQUIRE(pg.empty());
    queryObject->cleanUp();
}
