#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/PatternExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("PatternExtractor: Assign") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "assign-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"1", {"a", AstBuilder(TestDesignExtractorUtils::readFile("pattern/assign", "1.txt")).build()}},
            {"2", {"b", AstBuilder(TestDesignExtractorUtils::readFile("pattern/assign", "2.txt")).build()}},
            {"3", {"d", AstBuilder(TestDesignExtractorUtils::readFile("pattern/assign", "3.txt")).build()}},
            {"4", {"e", AstBuilder(TestDesignExtractorUtils::readFile("pattern/assign", "4.txt")).build()}},
            {"5", {"g", AstBuilder(TestDesignExtractorUtils::readFile("pattern/assign", "5.txt")).build()}},
            {"6", {"i", AstBuilder(TestDesignExtractorUtils::readFile("pattern/assign", "6.txt")).build()}},
            {"7", {"m", AstBuilder(TestDesignExtractorUtils::readFile("pattern/assign", "7.txt")).build()}},
            {"8", {"q", AstBuilder(TestDesignExtractorUtils::readFile("pattern/assign", "8.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
}

TEST_CASE("PatternExtractor: While") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "while-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"2", {"x", AstBuilder(TestDesignExtractorUtils::readFile("pattern/while", "2.txt")).build()}},
            {"9", {"w", AstBuilder(TestDesignExtractorUtils::readFile("pattern/while", "9.txt")).build()}},
            {"11", {"t", AstBuilder(TestDesignExtractorUtils::readFile("pattern/while", "11.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
}

TEST_CASE("PatternExtractor: If") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "if-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"2", {"x", AstBuilder(TestDesignExtractorUtils::readFile("pattern/if", "2.txt")).build()}},
            {"3", {"xx", AstBuilder(TestDesignExtractorUtils::readFile("pattern/if", "3.txt")).build()}},
            {"12", {"w", AstBuilder(TestDesignExtractorUtils::readFile("pattern/if", "12.txt")).build()}},
            {"13", {"ww", AstBuilder(TestDesignExtractorUtils::readFile("pattern/if", "13.txt")).build()}},
            {"15", {"r", AstBuilder(TestDesignExtractorUtils::readFile("pattern/if", "15.txt")).build()}},
            {"16", {"rr", AstBuilder(TestDesignExtractorUtils::readFile("pattern/if", "16.txt")).build()}},
            {"18", {"t", AstBuilder(TestDesignExtractorUtils::readFile("pattern/if", "18.txt")).build()}},
            {"19", {"tt", AstBuilder(TestDesignExtractorUtils::readFile("pattern/if", "19.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
}

TEST_CASE("PatternExtractor: Non-nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "non_nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"2", {"b", AstBuilder(TestDesignExtractorUtils::readFile("pattern/non-nested", "2.txt")).build()}},
            {"4", {"e", AstBuilder(TestDesignExtractorUtils::readFile("pattern/non-nested", "4.txt")).build()}},
            {"5", {"f", AstBuilder(TestDesignExtractorUtils::readFile("pattern/non-nested", "5.txt")).build()}},
            {"7", {"j", AstBuilder(TestDesignExtractorUtils::readFile("pattern/non-nested", "7.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
}

TEST_CASE("PatternExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"4", {"d", AstBuilder(TestDesignExtractorUtils::readFile("pattern/nested", "4.txt")).build()}},
            {"6", {"i", AstBuilder(TestDesignExtractorUtils::readFile("pattern/nested", "6.txt")).build()}},
            {"8", {"n", AstBuilder(TestDesignExtractorUtils::readFile("pattern/nested", "8.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
}
