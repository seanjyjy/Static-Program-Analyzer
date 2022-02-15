#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/ParentExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("ParentExtractor: Non-nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "non_nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    SECTION("Parent") {
        unordered_map<string, list<string>> expectedParent = {
                {"3", {"4", "5"}}, {"6", {"7", "8", "9"}}
        };
        REQUIRE(pe.getParentMap() == expectedParent);
    }

    SECTION("ParentT") {
        unordered_map<string, list<string>> expectedParentT = {
                {"3", {"4", "5"}}, {"6", {"7", "8", "9"}}
        };
        REQUIRE(pe.getParentTMap() == expectedParentT);
    }
}

TEST_CASE("ParentExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    ParentExtractor pe = ParentExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    SECTION("Parent") {
        unordered_map<string, list<string>> expectedParent = {
                {"1", {"2", "3"}}, {"3", {"4", "5", "6", "7"}}, {"7", {"8", "9", "10"}}
        };
        REQUIRE(pe.getParentMap() == expectedParent);
    }

    SECTION("ParentT") {
        unordered_map<string, list<string>> expectedParentT = {
                {"1", {"2", "3", "4", "5", "6", "7", "8", "9", "10"}},
                {"3", {"4", "5", "6", "7", "8", "9", "10"}}, {"7", {"8", "9", "10"}}
        };
        REQUIRE(pe.getParentTMap() == expectedParentT);
    }
}

