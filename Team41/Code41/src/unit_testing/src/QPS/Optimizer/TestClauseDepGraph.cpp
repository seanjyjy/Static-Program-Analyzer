#include "catch.hpp"
#include "QPS/Optimizer/ClauseDepGraph.h"
#include "QPS/QueryParser.h"
#include "TestOptimizerUtils.h"

TEST_CASE("Clause Dependency Graph") {
    ClauseDepGraph g;
    string pql = "variable v;\n"
                 "Select v such that Uses(\"p\", v)";
    QueryParser qp = QueryParser{pql};
    unique_ptr<QueryObject> queryObject(qp.parse());
    for (SuperClause *cl: queryObject->getSuperClauses()) {
        g.registerClause(cl);
    }
    vector<vector<SuperClause*>> clauses = g.split();
    sort(clauses.begin(), clauses.end(), [](auto &a, auto &b) {return a.size() < b.size();});
    REQUIRE(clauses.size() == 1);
    REQUIRE(clauses[0].size() == 1);
}