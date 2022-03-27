#include "catch.hpp"
#include "QPS/Optimizer/ClauseDepGraph.h"
#include "QPS/QueryParser.h"
#include "TestOptimizerUtils.h"

TEST_CASE("Clause Dependency Graph: basic") {
    ClauseDepGraph g;
    string pql = "variable v; stmt s;\n"
                 "Select v such that Uses(\"main\", v) and Modifies(s, \"x\")";
    QueryParser qp = QueryParser{pql};
    unique_ptr<QueryObject> queryObject(qp.parse());
    for (SuperClause *cl: queryObject->getSuperClauses()) {
        g.registerClause(cl);
    }
    vector<vector<SuperClause*>> clauseGrps = g.split();
    sort(clauseGrps.begin(), clauseGrps.end(), [](auto &a, auto &b) {return a.size() < b.size();});
    REQUIRE(clauseGrps.size() == 2);
    REQUIRE(clauseGrps[0].size() == 1); // Uses("p", v)
    REQUIRE(clauseGrps[1].size() == 1); // Modifies(s, "x")
}

TEST_CASE("Clause Dependency Graph: week 7 lecture example") {
    ClauseDepGraph g;
    string pql = "assign a1, a2, a3; stmt s1, s2, s3; variable v1, v2, v3;\n"
                 "Select <s1, s2, s3> such that Uses(s3, v1) and Modifies(s3, \"x\") and Follows(s1, s2) and Parent(s3, s1) and Uses(s2, v1) pattern a1(v1, _) pattern a2(v1, _) with v1.stmt# = 3";
    QueryParser qp = QueryParser{pql};
    unique_ptr<QueryObject> queryObject(qp.parse());
    for (SuperClause *cl: queryObject->getSuperClauses()) {
        g.registerClause(cl);
    }
    vector<vector<SuperClause*>> clauseGrps = g.split();
    sort(clauseGrps.begin(), clauseGrps.end(), [](auto &a, auto &b) {return a.size() < b.size();});
    REQUIRE(clauseGrps.size() == 1);
    REQUIRE(clauseGrps[0].size() == 8);
}

TEST_CASE("Clause Dependency Graph: should not remove duplicate clauses") {
    // duplicate clause removal should be the responsibility of the optimizer, not the clause dep graph
    ClauseDepGraph g;
    string pql = "assign a1, a2, a3; stmt s1, s2, s3; variable v1, v2, v3;\n"
                 "Select <s1, s2, s3> such that Uses(s3, v1) and Uses(s3, v1) and Uses(s3, v1) and Modifies(s3, \"x\") and Modifies(s3, \"x\") and Follows(s1, s2) and Parent(s3, s1) and Uses(s2, v1) pattern a2(v2, _) pattern a1(v1, _) pattern a1(v1, _) pattern a1(v1, _) with v1.stmt# = 3 with v1.stmt# = 3 with v1.stmt# = 3";
    QueryParser qp = QueryParser{pql};
    unique_ptr<QueryObject> queryObject(qp.parse());
    for (SuperClause *cl: queryObject->getSuperClauses()) {
        g.registerClause(cl);
    }
    vector<vector<SuperClause*>> clauseGrps = g.split();
    sort(clauseGrps.begin(), clauseGrps.end(), [](auto &a, auto &b) {return a.size() < b.size();});
    TestOptimizerUtils::print(clauseGrps);
    REQUIRE(clauseGrps.size() == 2);
    REQUIRE(clauseGrps[0].size() == 1); // pattern a1(v2, _)
    REQUIRE(clauseGrps[1].size() == 14);
}
