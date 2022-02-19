#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/ParentExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("ParentExtractor: Assign") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("assign-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    REQUIRE(pe.getParentMap().empty());
    REQUIRE(pe.getParentTMap().empty());
}

TEST_CASE("ParentExtractor: Read") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("read-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    REQUIRE(pe.getParentMap().empty());
    REQUIRE(pe.getParentTMap().empty());
}

TEST_CASE("ParentExtractor: Print") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("print-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    REQUIRE(pe.getParentMap().empty());
    REQUIRE(pe.getParentTMap().empty());
}

TEST_CASE("ParentExtractor: While") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("while-xml.txt")).build();
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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("if-xml.txt")).build();
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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("non_nested-xml.txt")).build();
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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("nested-xml.txt")).build();
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

TEST_CASE("ParentExtractor: n3iif") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3iif.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, list<string>> expectedParent = {
            {"1", {"2", "3", "4"}}, {"4", {"5", "11", "12", "13", "16"}}, {"5", {"6", "9", "10"}},
            {"6", {"7", "8"}}, {"13", {"14", "15"}}
    };
    REQUIRE(pe.getParentMap() == expectedParent);
    unordered_map<string, list<string>> expectedParentT = {
            {"1", {"2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"}},
            {"4", {"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"}},
            {"5", {"6", "7", "8", "9", "10"}}, {"6", {"7", "8"}}, {"13", {"14", "15"}}
    };
    REQUIRE(pe.getParentTMap() == expectedParentT);
}

TEST_CASE("ParentExtractor: n3iwl") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3iwl.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, list<string>> expectedParent = {
            {"2", {"3", "4", "13"}}, {"4", {"5", "6"}}, {"6", {"7", "8", "10", "11"}}, {"8", {"9"}}, {"11", {"12"}}
    };
    REQUIRE(pe.getParentMap() == expectedParent);
    unordered_map<string, list<string>> expectedParentT = {
            {"2", {"3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"}},
            {"4", {"5", "6", "7", "8", "9", "10", "11", "12"}},
            {"6", {"7", "8", "9", "10", "11", "12"}}, {"8", {"9"}}, {"11", {"12"}}
    };
    REQUIRE(pe.getParentTMap() == expectedParentT);
}

TEST_CASE("ParentExtractor: n3wim") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3wim.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, list<string>> expectedParent = {
            {"2", {"3", "4", "21"}}, {"4", {"5", "6", "12", "13", "14", "20"}}, {"6", {"7", "8", "11"}},
            {"8", {"9", "10"}}, {"14", {"15", "16", "19"}}, {"16", {"17", "18"}}
    };
    REQUIRE(pe.getParentMap() == expectedParent);
    unordered_map<string, list<string>> expectedParentT = {
            {"2", {"3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21"}},
            {"4", {"5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20"}},
            {"6", {"7", "8", "9", "10", "11"}}, {"8", {"9", "10"}},
            {"14", {"15", "16", "17", "18", "19"}}, {"16", {"17", "18"}}
    };
    REQUIRE(pe.getParentTMap() == expectedParentT);
}
