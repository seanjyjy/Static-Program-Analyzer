#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/ModifiesExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("ModifiesExtractor: Non-nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "non_nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap);
    me.extractRelationship();

    SECTION("Procedure Modifies") {
        unordered_map<string, unordered_set<string>> expectedProcModifies = {
                {"main", {"a", "b", "e", "f", "j", "m"}}
        };
        REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    }

    SECTION("Statement Modifies") {
        unordered_map<string, unordered_set<string>> expectedStmtModifies = {
                {"1", {"a"}}, {"2", {"b"}}, {"3", {"e", "f"}}, {"4", {"e"}}, {"5", {"f"}},
                {"6", {"j", "m"}}, {"7", {"j"}}, {"9", {"m"}}
        };
        REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    }
}

TEST_CASE("ModifiesExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap);
    me.extractRelationship();

    SECTION("Procedure Modifies") {
        unordered_map<string, unordered_set<string>> expectedProcModifies = {
                {"main", {"b", "d", "i", "n", "s"}}
        };
        REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    }

    SECTION("Statement Modifies") {
        unordered_map<string, unordered_set<string>> expectedStmtModifies = {
                {"1", {"b", "d", "i", "n", "s"}}, {"2", {"b"}}, {"3", {"d", "i", "n", "s"}},
                {"4", {"d"}}, {"6", {"i"}}, {"7", {"n", "s"}}, {"8", {"n"}}, {"9", {"s"}}
        };
        REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    }
}
