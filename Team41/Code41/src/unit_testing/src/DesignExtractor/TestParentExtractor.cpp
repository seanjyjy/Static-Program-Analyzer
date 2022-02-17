#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/ParentExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("ParentExtractor: Assign") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "assign-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    REQUIRE(pe.getParentMap().empty());
    REQUIRE(pe.getParentTMap().empty());
}

TEST_CASE("ParentExtractor: Read") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "read-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    REQUIRE(pe.getParentMap().empty());
    REQUIRE(pe.getParentTMap().empty());
}

TEST_CASE("ParentExtractor: Print") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "print-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    REQUIRE(pe.getParentMap().empty());
    REQUIRE(pe.getParentTMap().empty());
}

TEST_CASE("ParentExtractor: While") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "while-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, list<string>> expectedParent = {
            {"1", {"2"}}, {"3", {"4"}}, {"5", {"6"}}, {"7", {"8"}}, {"8", {"9"}}, {"10", {"11"}}
    };
    REQUIRE(pe.getParentMap() == expectedParent);
    unordered_map<string, list<string>> expectedParentT = {
            {"1", {"2"}}, {"3", {"4"}}, {"5", {"6"}}, {"7", {"8", "9"}}, {"8", {"9"}}, {"10", {"11"}}
    };
    REQUIRE(pe.getParentTMap() == expectedParentT);
}

TEST_CASE("ParentExtractor: If") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "if-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, list<string>> expectedParent = {
            {"1", {"2", "3"}}, {"4", {"5", "6"}}, {"7", {"8", "9"}},
            {"10", {"11", "14"}}, {"11", {"12", "13"}}, {"14", {"15", "16"}}, {"17", {"18", "19"}}
    };
    REQUIRE(pe.getParentMap() == expectedParent);
    unordered_map<string, list<string>> expectedParentT = {
            {"1", {"2", "3"}}, {"4", {"5", "6"}}, {"7", {"8", "9"}},
            {"10", {"11", "12", "13", "14", "15", "16"}}, {"11", {"12", "13"}}, {"14", {"15", "16"}}, {"17", {"18", "19"}}
    };
    REQUIRE(pe.getParentTMap() == expectedParentT);
}

TEST_CASE("ParentExtractor: Non-nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "non_nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, list<string>> expectedParent = {
            {"3", {"4", "5"}}, {"6", {"7", "8", "9"}}
    };
    REQUIRE(pe.getParentMap() == expectedParent);
    unordered_map<string, list<string>> expectedParentT = {
            {"3", {"4", "5"}}, {"6", {"7", "8", "9"}}
    };
    REQUIRE(pe.getParentTMap() == expectedParentT);
}

TEST_CASE("ParentExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, list<string>> expectedParent = {
            {"1", {"2", "3"}}, {"3", {"4", "5", "6", "7"}}, {"7", {"8", "9", "10"}}
    };
    REQUIRE(pe.getParentMap() == expectedParent);
    unordered_map<string, list<string>> expectedParentT = {
            {"1", {"2", "3", "4", "5", "6", "7", "8", "9", "10"}},
            {"3", {"4", "5", "6", "7", "8", "9", "10"}}, {"7", {"8", "9", "10"}}
    };
    REQUIRE(pe.getParentTMap() == expectedParentT);
}

