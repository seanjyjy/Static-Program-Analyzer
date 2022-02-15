#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/UsesExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("UsesExtractor: Non-nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "non_nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    SECTION("Procedure Uses") {
        unordered_map<string, unordered_set<string>> expectedProcUses = {
                {"main", {"c", "d", "g", "h", "i", "k", "n"}}
        };
        REQUIRE(ue.getProcUsesMap() == expectedProcUses);
    }

    SECTION("Statement Uses") {
        unordered_map<string, unordered_set<string>> expectedStmtUses = {
                {"2", {"c"}}, {"3", {"d"}}, {"6", {"g", "h", "i", "k"}}, {"8", {"k"}}, {"10", {"n"}}
        };
        REQUIRE(ue.getStmtUsesMap() == expectedStmtUses);
    }
}

TEST_CASE("UsesExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    SECTION("Procedure Uses") {
        unordered_map<string, unordered_set<string>> expectedProcUses = {
                {"main", {"a", "c", "e", "f", "g", "h", "j", "k", "m", "p", "q", "r", "t"}}
        };
        REQUIRE(ue.getProcUsesMap() == expectedProcUses);
    }

    SECTION("Statement Uses") {
        unordered_map<string, unordered_set<string>> expectedStmtUses = {
                {"1", {"a", "c", "e", "f", "g", "h", "j", "k", "m", "p", "q", "r", "t"}},
                {"3", {"c", "e", "f", "g", "h", "j", "k", "m", "p", "q", "r", "t"}},
                {"4", {"e", "f", "g"}}, {"5", {"h"}},
                {"7", {"j", "k", "m", "p", "q", "r", "t"}},
                {"8", {"p", "q", "r"}}, {"10", {"t"}}
        };
        REQUIRE(ue.getStmtUsesMap() == expectedStmtUses);
    }
}

