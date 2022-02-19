#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/FollowsExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("FollowsExtractor: While") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("while.x")).build();
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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("if.x")).build();
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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("non_nested.x")).build();
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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("nested.x")).build();
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

TEST_CASE("FollowsExtractor: n3iif") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3iif.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extractRelationship();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1", {"17", "18"}}, {"17", {"18"}}, {"2", {"3"}}, {"5", {"11", "12"}}, {"11", {"12"}},
            {"6", {"9"}}, {"13", {"16"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: n3iwl") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3iwl.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extractRelationship();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1", {"2"}}, {"3", {"4"}}, {"5", {"6"}}, {"7", {"8"}}, {"10", {"11"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: n3wim") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3wim.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extractRelationship();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1", {"2", "22"}}, {"2", {"22"}}, {"3", {"4", "21"}}, {"4", {"21"}},
            {"5", {"6", "12"}}, {"6", {"12"}}, {"7", {"8", "11"}}, {"8", {"11"}},
            {"13", {"14", "20"}}, {"14", {"20"}}, {"15", {"16", "19"}}, {"16", {"19"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}

TEST_CASE("FollowsExtractor: n3wwl") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3wwl.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    FollowsExtractor fe = FollowsExtractor(ast, nodeToStmtNumMap);
    fe.extractRelationship();

    unordered_map<string, list<string>> expectedFollowsT = {
            {"1", {"2"}}, {"3", {"4"}}, {"5", {"6"}}, {"7", {"8"}}
    };
    REQUIRE(fe.getFollowsTMap() == expectedFollowsT);
    delete ast;
}
