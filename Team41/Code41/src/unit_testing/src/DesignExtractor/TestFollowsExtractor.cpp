#include "catch.hpp"

#include "DesignExtractor/FollowsExtractor.h"
#include "Common/AstBuilder.h"
#include "TestExtractorUtils.h"

using namespace std;

/**
 * Assign, Read, Print, While, If, Non-nested, Nested, n3iif, n3iwl, n3wim, n3wwl
 * multiproc1
 */

TEST_CASE("FollowsExtractor: Assign") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("assign.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extract();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1", {"2", "3", "4", "5", "6", "7", "8"}},
            {"2", {"3", "4", "5", "6", "7", "8"}},
            {"3", {"4", "5", "6", "7", "8"}},
            {"4", {"5", "6", "7", "8"}},
            {"5", {"6", "7", "8"}},
            {"6", {"7", "8"}},
            {"7", {"8"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: Read") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("read.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extract();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1", {"2", "3"}},
            {"2", {"3"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: Print") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("print.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extract();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1", {"2", "3"}},
            {"2", {"3"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: While") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("while.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extract();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1", {"3", "5", "7", "10"}},
            {"3", {"5", "7", "10"}},
            {"5", {"7", "10"}},
            {"7", {"10"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: If") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("if.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extract();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1",  {"4",  "7",  "10", "17"}},
            {"4",  {"7",  "10", "17"}},
            {"7",  {"10", "17"}},
            {"10", {"17"}},
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: Non-nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("non_nested.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extract();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1", {"2", "3", "6", "10"}},
            {"2", {"3", "6", "10"}},
            {"3", {"6", "10"}},
            {"6", {"10"}},
            {"4", {"5"}},
            {"8", {"9"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("nested.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extract();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"2", {"3"}},
            {"4", {"5"}},
            {"6", {"7"}},
            {"8", {"9", "10"}},
            {"9", {"10"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: n3iif") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3iif.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extract();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1",  {"17", "18"}},
            {"17", {"18"}},
            {"2",  {"3"}},
            {"5",  {"11", "12"}},
            {"11", {"12"}},
            {"6",  {"9"}},
            {"13", {"16"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: n3iwl") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3iwl.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extract();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1",  {"2"}},
            {"3",  {"4"}},
            {"5",  {"6"}},
            {"7",  {"8"}},
            {"10", {"11"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: n3wim") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3wim.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extract();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1",  {"2",  "22"}},
            {"2",  {"22"}},
            {"3",  {"4",  "21"}},
            {"4",  {"21"}},
            {"5",  {"6",  "12"}},
            {"6",  {"12"}},
            {"7",  {"8",  "11"}},
            {"8",  {"11"}},
            {"13", {"14", "20"}},
            {"14", {"20"}},
            {"15", {"16", "19"}},
            {"16", {"19"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: n3wwl") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3wwl.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extract();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1", {"2"}},
            {"3", {"4"}},
            {"5", {"6"}},
            {"7", {"8"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: multiproc1") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("multiproc/multiproc1.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extract();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1",  {"8",  "9"}},
            {"2",  {"5"}},
            {"6",  {"7"}},
            {"8",  {"9"}},
            {"10", {"13", "14"}},
            {"11", {"12"}},
            {"13", {"14"}},
            {"15", {"16", "19"}},
            {"16", {"19"}},
            {"17", {"18"}},
            {"20", {"21", "22"}},
            {"21", {"22"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}
