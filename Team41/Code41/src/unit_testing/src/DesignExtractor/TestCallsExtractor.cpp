#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/CallsExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

/**
 * Call
 */

TEST_CASE("CallsExtractor: Call") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("call.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestDesignExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_set<string> procSet = {"p1", "p2", "p3", "p4", "p5", "p6"};
    CallsExtractor ce = CallsExtractor(ast, procSet);
    ce.extract();

    unordered_map<string, unordered_set<string>> expectedCalls = {
            {"p1", {"p2", "p4"}}, {"p2", {"p3"}}, {"p4", {"p5", "p6"}}, {"p5", {"p6"}}
    };
    REQUIRE(ce.getCallsMap() == expectedCalls);
    unordered_map<string, unordered_set<string>> expectedCallsT = {
            {"p1", {"p2", "p3", "p4", "p5", "p6"}}, {"p2", {"p3"}}, {"p4", {"p5", "p6"}}, {"p5", {"p6"}}
    };
    REQUIRE(ce.getCallsTMap() == expectedCallsT);
    REQUIRE(TestDesignExtractorUtils::checkCallsOrder(ce.getProcCallOrder(), expectedCalls));
    delete ast;
}
