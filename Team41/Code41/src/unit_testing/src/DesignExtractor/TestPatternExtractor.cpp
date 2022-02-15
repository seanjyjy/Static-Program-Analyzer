#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/PatternExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("PatternExtractor: Non-nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "non_nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();

    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    SECTION("AssignPattern") {
        unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
                {"2", {"b", AstBuilder(TestDesignExtractorUtils::readFile("pattern", "2.txt")).build()}},
                {"4", {"e", AstBuilder(TestDesignExtractorUtils::readFile("pattern", "4.txt")).build()}},
                {"5", {"f", AstBuilder(TestDesignExtractorUtils::readFile("pattern", "5.txt")).build()}},
                {"7", {"j", AstBuilder(TestDesignExtractorUtils::readFile("pattern", "7.txt")).build()}}
        };
        REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
    }
}
