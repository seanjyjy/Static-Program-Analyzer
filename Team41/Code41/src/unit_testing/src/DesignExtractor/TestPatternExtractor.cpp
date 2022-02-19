#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/PatternExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("PatternExtractor: Assign") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("assign.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"1", {"a", AstBuilder(TestDesignExtractorUtils::readDePattern("assign", "1.txt")).build()}},
            {"2", {"b", AstBuilder(TestDesignExtractorUtils::readDePattern("assign", "2.txt")).build()}},
            {"3", {"d", AstBuilder(TestDesignExtractorUtils::readDePattern("assign", "3.txt")).build()}},
            {"4", {"e", AstBuilder(TestDesignExtractorUtils::readDePattern("assign", "4.txt")).build()}},
            {"5", {"g", AstBuilder(TestDesignExtractorUtils::readDePattern("assign", "5.txt")).build()}},
            {"6", {"i", AstBuilder(TestDesignExtractorUtils::readDePattern("assign", "6.txt")).build()}},
            {"7", {"m", AstBuilder(TestDesignExtractorUtils::readDePattern("assign", "7.txt")).build()}},
            {"8", {"q", AstBuilder(TestDesignExtractorUtils::readDePattern("assign", "8.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
}

TEST_CASE("PatternExtractor: While") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("while.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"2", {"x", AstBuilder(TestDesignExtractorUtils::readDePattern("while", "2.txt")).build()}},
            {"9", {"w", AstBuilder(TestDesignExtractorUtils::readDePattern("while", "9.txt")).build()}},
            {"11", {"t", AstBuilder(TestDesignExtractorUtils::readDePattern("while", "11.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
}

TEST_CASE("PatternExtractor: If") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("if.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"2", {"x", AstBuilder(TestDesignExtractorUtils::readDePattern("if", "2.txt")).build()}},
            {"3", {"xx", AstBuilder(TestDesignExtractorUtils::readDePattern("if", "3.txt")).build()}},
            {"12", {"w", AstBuilder(TestDesignExtractorUtils::readDePattern("if", "12.txt")).build()}},
            {"13", {"ww", AstBuilder(TestDesignExtractorUtils::readDePattern("if", "13.txt")).build()}},
            {"15", {"r", AstBuilder(TestDesignExtractorUtils::readDePattern("if", "15.txt")).build()}},
            {"16", {"rr", AstBuilder(TestDesignExtractorUtils::readDePattern("if", "16.txt")).build()}},
            {"18", {"t", AstBuilder(TestDesignExtractorUtils::readDePattern("if", "18.txt")).build()}},
            {"19", {"tt", AstBuilder(TestDesignExtractorUtils::readDePattern("if", "19.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
}

TEST_CASE("PatternExtractor: Non-nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("non_nested.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"2", {"b", AstBuilder(TestDesignExtractorUtils::readDePattern("non-nested", "2.txt")).build()}},
            {"4", {"e", AstBuilder(TestDesignExtractorUtils::readDePattern("non-nested", "4.txt")).build()}},
            {"5", {"f", AstBuilder(TestDesignExtractorUtils::readDePattern("non-nested", "5.txt")).build()}},
            {"7", {"j", AstBuilder(TestDesignExtractorUtils::readDePattern("non-nested", "7.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
}

TEST_CASE("PatternExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("nested.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extractRelationship();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"4", {"d", AstBuilder(TestDesignExtractorUtils::readDePattern("nested", "4.txt")).build()}},
            {"6", {"i", AstBuilder(TestDesignExtractorUtils::readDePattern("nested", "6.txt")).build()}},
            {"8", {"n", AstBuilder(TestDesignExtractorUtils::readDePattern("nested", "8.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
}
