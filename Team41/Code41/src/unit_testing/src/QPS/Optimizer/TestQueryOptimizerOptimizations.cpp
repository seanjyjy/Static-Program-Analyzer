#include "catch.hpp"
#include "QPS/Optimizer/QueryOptimizer.h"
#include "QPS/QueryParser.h"
#include "TestOptimizerUtils.h"

#include <string>

using namespace std;

TEST_CASE("Query Optimizer: Simplifiable groups") {
    string query = "stmt s1, s2, s3, s4, s5, s6;\n"
                   "Select BOOLEAN such that Affects*(s1, s2) and Affects*(s3, _) and Affects*(_, s4) and Affects*(s5, s6)";
    // parse the query and setup objects
    PKBManager pkbManager;
    QueryParser qp = QueryParser{query};
    QueryObject *qo(qp.parse());
    OptimizedQueryObject oqo = QueryOptimizer::create()
            .enableAllOptimizations(&pkbManager)
            .optimize(qo);
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.isLastOfGroup() == true);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.isLastOfGroup() == true);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.isLastOfGroup() == true);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.isLastOfGroup() == true);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.empty());

    delete qo;
}

TEST_CASE("Query Optimizer: Simplifiable groups 2") {
    string query = "stmt s1, s2, s3, s4, s5, s6;\n"
                   "Select BOOLEAN such that Affects(s1, s2) and Affects(s3, _) and Affects(_, s4) and Affects(s5, s6)";
    // parse the query and setup objects
    PKBManager pkbManager;
    QueryParser qp = QueryParser{query};
    QueryObject *qo(qp.parse());
    OptimizedQueryObject oqo = QueryOptimizer::create()
            .enableAllOptimizations(&pkbManager)
            .optimize(qo);
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.isLastOfGroup() == true);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.isLastOfGroup() == true);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.isLastOfGroup() == true);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.isLastOfGroup() == true);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.empty());

    delete qo;
}

TEST_CASE("Query Optimizer: Simplifiable groups 3") {
    string query = "stmt s1, s2, s3, s4, s5, s6;\n"
                   "Select <s1, s2> such that Follows(s1, s2) and Follows(s2, s3) and Follows(s3, s4) and Follows(s4, s5) and Follows(s5, s6)";
    // parse the query and setup objects
    PKBManager pkbManager;
    QueryParser qp = QueryParser{query};
    QueryObject *qo(qp.parse());
    OptimizedQueryObject oqo = QueryOptimizer::create()
            .enableAllOptimizations(&pkbManager)
            .optimize(qo);
    REQUIRE(oqo.currGroupCanSimplify() == false);
    REQUIRE(oqo.currGroupSize() == 5);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.isLastOfGroup());
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.empty());

    delete qo;
}

// https://github.com/nus-cs3203/21s2-cp-spa-team-41/issues/287
// this will probably break easily if we change our sort logic. beware!
TEST_CASE("QueryOptimizer: Simplifiable Groups sort order - select synonyms") {
    string query = "procedure p; stmt s1, s2, s3, s4, s5, s6, s7; variable v1, v2, v5, v6, v6, v7;\n"
                   "Select p such that Uses(s1, v1) and Modifies(s2, v1) and Uses(s3, v2) and Modifies(s4, v2) and Uses(s5, v5) and Uses(s6, v6) and Uses(s7, v7) and Uses(p, \"x\") and Modifies(p, \"y\")";
    // parse the query and setup objects
    PKBManager pkbManager;
    QueryParser qp = QueryParser{query};
    QueryObject *qo(qp.parse());
    OptimizedQueryObject oqo = QueryOptimizer::create()
            .enableAllOptimizations(&pkbManager)
            .optimize(qo);
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.isLastOfGroup());
    REQUIRE(oqo.currGroupSize() == 1);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.isLastOfGroup());
    REQUIRE(oqo.currGroupSize() == 1);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.isLastOfGroup());
    REQUIRE(oqo.currGroupSize() == 1);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.currGroupSize() == 2);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.isLastOfGroup());
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.currGroupSize() == 2);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.isLastOfGroup());
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == false);
    REQUIRE(oqo.currGroupSize() == 2);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.isLastOfGroup());
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.empty());

    delete qo;
}

TEST_CASE("QueryOptimizer: Simplifiable Groups sort order - select BOOLEAN") {
    string query = "procedure p; stmt s1, s2, s3, s4, s5, s6, s7; variable v1, v2, v5, v6, v6, v7;\n"
                   "Select BOOLEAN such that Uses(s1, v1) and Modifies(s2, v1) and Uses(s3, v2) and Modifies(s4, v2) and Uses(s5, v5) and Uses(s6, v6) and Uses(s7, v7) and Uses(p, \"x\") and Modifies(p, \"y\")";
    // parse the query and setup objects
    PKBManager pkbManager;
    QueryParser qp = QueryParser{query};
    QueryObject *qo(qp.parse());
    OptimizedQueryObject oqo = QueryOptimizer::create()
            .enableAllOptimizations(&pkbManager)
            .optimize(qo);
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.isLastOfGroup());
    REQUIRE(oqo.currGroupSize() == 1);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.isLastOfGroup());
    REQUIRE(oqo.currGroupSize() == 1);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.isLastOfGroup());
    REQUIRE(oqo.currGroupSize() == 1);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.currGroupSize() == 2);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.isLastOfGroup());
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.currGroupSize() == 2);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.isLastOfGroup());
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.currGroupCanSimplify() == true);
    REQUIRE(oqo.currGroupSize() == 2);
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.isLastOfGroup());
    REQUIRE_NOTHROW(oqo.popClause());
    REQUIRE(oqo.empty());

    delete qo;
}