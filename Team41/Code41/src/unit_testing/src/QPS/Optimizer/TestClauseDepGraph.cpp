#include "catch.hpp"
#include "QPS/Optimizer/ClauseDepGraph.h"
#include "TestOptimizerUtils.h"

TEST_CASE("Clause Dependency Graph: basic") {
    string pql = "variable v; stmt s;\n"
                 "Select v such that Uses(\"main\", v) and Modifies(s, \"x\")";
    vector<vector<SuperClause*>> clauseGrps = TestOptimizerUtils::prepareClauseGroups(pql);
    sort(clauseGrps.begin(), clauseGrps.end(), [](auto &a, auto &b) {return a.size() < b.size();});
    REQUIRE(clauseGrps.size() == 2);
    REQUIRE(clauseGrps[0].size() == 1); // Uses("p", v)
    REQUIRE(clauseGrps[1].size() == 1); // Modifies(s, "x")
}

TEST_CASE("Clause Dependency Graph: complex") {
    string pql = "assign a1, a2, a3; stmt s1, s2, s3; variable v1, v2, v3;\n"
                 "Select <s1, s2, s3> such that Uses(s3, v1) and Modifies(s3, \"x\") and Follows(s1, s2) "
                 "and Parent(s3, s1) and Uses(s2, v1) pattern a1(v1, _) pattern a2(v1, _) with v1.stmt# = 3";
    vector<vector<SuperClause*>> clauseGrps = TestOptimizerUtils::prepareClauseGroups(pql);
    sort(clauseGrps.begin(), clauseGrps.end(), [](auto &a, auto &b) {return a.size() < b.size();});
    REQUIRE(clauseGrps.size() == 1);
    REQUIRE(clauseGrps[0].size() == 8);
}

TEST_CASE("Clause Dependency Graph: week 7 lecture example") {
    string pql = "assign a1, a2, a3; stmt s1, s2, s3; variable v1, v2, v3;\n"
                   "Select <s1, s2, s3> such that Uses(s3, v1) and Modifies(s3, \"x\") and Follows(s1, s2) "
                   "and Parent(s3, s1) and Uses(s2, v1) such that Uses(5, \"y\") and Follows(3, 4) "
                   "pattern a1(v2, _\"x+y\"_) such that Affects(a1, a2) with a2.stmt# = 20 "
                   "such that Modifies(a3, v3) pattern a3(\"z\", _)";
    vector<vector<SuperClause*>> clauseGrps = TestOptimizerUtils::prepareClauseGroups(pql);
    sort(clauseGrps.begin(), clauseGrps.end(), [](auto &a, auto &b) {return a.size() < b.size();});
    // Follows(3,4), Uses(5,"y")
    // pattern a3("z",_) Modifies(a3, v3)
    // with a2.stmt# = 20 Affects(a1, a2) pattern a1(v2, _"x+y"_)
    // Modifies(s3,"x") Parent(s3,s1) Follows(s1,s2) Uses(s3,v1) Uses(s2,v1)
    REQUIRE(clauseGrps.size() == 4);
    REQUIRE(clauseGrps[0].size() == 2);
    REQUIRE(clauseGrps[1].size() == 2);
    REQUIRE(clauseGrps[2].size() == 3);
    REQUIRE(clauseGrps[3].size() == 5);
}

TEST_CASE("Clause Dependency Graph: should not remove duplicate clauses") {
    // duplicate clause removal should be the responsibility of the optimizer, not the clause dep graph
    string pql = "assign a1, a2, a3; stmt s1, s2, s3; variable v1, v2, v3;\n"
                 "Select <s1, s2, s3> such that Uses(s3, v1) and Uses(s3, v1) and Uses(s3, v1) and Modifies(s3, \"x\") and Modifies(s3, \"x\") and Follows(s1, s2) and Parent(s3, s1) and Uses(s2, v1) pattern a2(v2, _) pattern a1(v1, _) pattern a1(v1, _) pattern a1(v1, _) with v1.stmt# = 3 with v1.stmt# = 3 with v1.stmt# = 3";
    vector<vector<SuperClause*>> clauseGrps = TestOptimizerUtils::prepareClauseGroups(pql);
    sort(clauseGrps.begin(), clauseGrps.end(), [](auto &a, auto &b) {return a.size() < b.size();});
    REQUIRE(clauseGrps.size() == 2);
    REQUIRE(clauseGrps[0].size() == 1); // pattern a1(v2, _)
    REQUIRE(clauseGrps[1].size() == 14);
}
