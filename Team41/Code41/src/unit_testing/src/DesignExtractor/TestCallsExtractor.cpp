#include "catch.hpp"

#include "DesignExtractor/CallsExtractor.h"
#include "Common/AstBuilder.h"
#include "TestExtractorUtils.h"

using namespace std;

/**
 * Call
 * multiproc1, multiproc2, multiproc3, multiproc4
 */

TEST_CASE("CallsExtractor: Call") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("call.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_set<string> procSet = {"p1", "p2", "p3", "p4", "p5", "p6"};
    CallsExtractor ce = CallsExtractor(ast, procSet);
    ce.extract();

    unordered_map<string, unordered_set<string>> expectedCalls = {
            {"p1", {"p2", "p4"}},
            {"p2", {"p3"}},
            {"p4", {"p5", "p6"}},
            {"p5", {"p6"}}
    };
    REQUIRE(ce.getCallsMap() == expectedCalls);
    unordered_map<string, unordered_set<string>> expectedCallsT = {
            {"p1", {"p2", "p3", "p4", "p5", "p6"}},
            {"p2", {"p3"}},
            {"p4", {"p5", "p6"}},
            {"p5", {"p6"}}
    };
    REQUIRE(ce.getCallsTMap() == expectedCallsT);
    REQUIRE(TestExtractorUtils::checkCallsOrder(ce.getProcCallOrder(), expectedCalls));
    delete ast;
}

TEST_CASE("CallsExtractor: multiproc1") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("multiproc/multiproc1.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_set<string> procSet = {"n1iif", "n0f", "n0m", "n0l"};
    CallsExtractor ce = CallsExtractor(ast, procSet);
    ce.extract();

    unordered_map<string, unordered_set<string>> expectedCalls = {
            {"n1iif", {"n0f"}},
            {"n0f",   {"n0m"}},
            {"n0m",   {"n0l"}}
    };
    REQUIRE(ce.getCallsMap() == expectedCalls);
    unordered_map<string, unordered_set<string>> expectedCallsT = {
            {"n1iif", {"n0f", "n0m", "n0l"}},
            {"n0f",   {"n0m", "n0l"}},
            {"n0m",   {"n0l"}}
    };
    REQUIRE(ce.getCallsTMap() == expectedCallsT);
    REQUIRE(TestExtractorUtils::checkCallsOrder(ce.getProcCallOrder(), expectedCalls));
    delete ast;
}

TEST_CASE("CallsExtractor: multiproc2") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("multiproc/multiproc2.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_set<string> procSet = {"n1iif", "n0f", "n0m", "n0l"};
    CallsExtractor ce = CallsExtractor(ast, procSet);
    ce.extract();

    unordered_map<string, unordered_set<string>> expectedCalls = {
            {"n1iif", {"n0f", "n0m"}},
            {"n0f",   {"n0l"}},
            {"n0m",   {"n0l"}}
    };
    REQUIRE(ce.getCallsMap() == expectedCalls);
    unordered_map<string, unordered_set<string>> expectedCallsT = {
            {"n1iif", {"n0f", "n0m", "n0l"}},
            {"n0f",   {"n0l"}},
            {"n0m",   {"n0l"}}
    };
    REQUIRE(ce.getCallsTMap() == expectedCallsT);
    REQUIRE(TestExtractorUtils::checkCallsOrder(ce.getProcCallOrder(), expectedCalls));
    delete ast;
}

TEST_CASE("CallsExtractor: multiproc3") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("multiproc/multiproc3.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_set<string> procSet = {"n1iif", "n0f", "n0m", "n0l"};
    CallsExtractor ce = CallsExtractor(ast, procSet);
    ce.extract();

    unordered_map<string, unordered_set<string>> expectedCalls = {
            {"n1iif", {"n0f", "n0m", "n0l"}}
    };
    REQUIRE(ce.getCallsMap() == expectedCalls);
    unordered_map<string, unordered_set<string>> expectedCallsT = {
            {"n1iif", {"n0f", "n0m", "n0l"}}
    };
    REQUIRE(ce.getCallsTMap() == expectedCallsT);
    REQUIRE(TestExtractorUtils::checkCallsOrder(ce.getProcCallOrder(), expectedCalls));
    delete ast;
}

TEST_CASE("CallsExtractor: multiproc4") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("multiproc/multiproc4.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_set<string> procSet = {"n1iif", "n0f", "n0m", "n0l"};
    CallsExtractor ce = CallsExtractor(ast, procSet);
    ce.extract();

    unordered_map<string, unordered_set<string>> expectedCalls = {
            {"n1iif", {"n0f"}},
            {"n0m",   {"n0l"}}
    };
    REQUIRE(ce.getCallsMap() == expectedCalls);
    unordered_map<string, unordered_set<string>> expectedCallsT = {
            {"n1iif", {"n0f"}},
            {"n0m",   {"n0l"}}
    };
    REQUIRE(ce.getCallsTMap() == expectedCallsT);
    REQUIRE(TestExtractorUtils::checkCallsOrder(ce.getProcCallOrder(), expectedCalls));
    delete ast;
}
