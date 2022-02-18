#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/FollowsExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("FollowsExtractor: While") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "while-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extractRelationship();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1", {"3", "5", "7", "10"}}, {"3", {"5", "7", "10"}}, {"5", {"7", "10"}}, {"7", {"10"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: If") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "if-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extractRelationship();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1", {"4", "7", "10", "17"}}, {"4", {"7", "10", "17"}}, {"7", {"10", "17"}}, {"10", {"17"}},
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: Non-nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "non_nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extractRelationship();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1", {"2", "3", "6", "10"}}, {"2", {"3", "6", "10"}}, {"3", {"6", "10"}},
            {"6", {"10"}}, {"4", {"5"}}, {"8", {"9"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extractRelationship();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"2", {"3"}}, {"4", {"5"}}, {"6", {"7"}}, {"8", {"9", "10"}}, {"9", {"10"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

