#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/PatternExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

/**
 * Assign, Read, Print, While, If, Non-nested, Nested, n3iif, n3iwl, n3wim, n3wwl
 */

TEST_CASE("PatternExtractor: Assign") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("assign.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

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
    REQUIRE(pe.getIfPatternMap().empty());
    REQUIRE(pe.getWhilePatternMap().empty());
    for (auto[stmt, expectedPair]: expectedAssignPattern)
        delete expectedPair.second;
    delete ast;
}

TEST_CASE("PatternExtractor: Read") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("read.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    REQUIRE(pe.getAssignPatternMap().empty());
    REQUIRE(pe.getIfPatternMap().empty());
    REQUIRE(pe.getWhilePatternMap().empty());
    delete ast;
}

TEST_CASE("PatternExtractor: Print") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("print.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    REQUIRE(pe.getAssignPatternMap().empty());
    REQUIRE(pe.getIfPatternMap().empty());
    REQUIRE(pe.getWhilePatternMap().empty());
    delete ast;
}

TEST_CASE("PatternExtractor: While") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("while.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"2",  {"x", AstBuilder(TestDesignExtractorUtils::readDePattern("while", "2.txt")).build()}},
            {"9",  {"w", AstBuilder(TestDesignExtractorUtils::readDePattern("while", "9.txt")).build()}},
            {"11", {"t", AstBuilder(TestDesignExtractorUtils::readDePattern("while", "11.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
    REQUIRE(pe.getIfPatternMap().empty());
    unordered_map<string, unordered_set<string>> expectedWhilePattern = {
            {"1",  {"a"}},
            {"3",  {"b"}},
            {"5",  {"c", "d"}},
            {"7",  {"e"}},
            {"8",  {"f"}},
            {"10", {"g", "h"}}
    };
    REQUIRE(pe.getWhilePatternMap() == expectedWhilePattern);
    for (auto[stmt, expectedPair]: expectedAssignPattern)
        delete expectedPair.second;
    delete ast;
}

TEST_CASE("PatternExtractor: If") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("if.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"2",  {"x",  AstBuilder(TestDesignExtractorUtils::readDePattern("if", "2.txt")).build()}},
            {"3",  {"xx", AstBuilder(TestDesignExtractorUtils::readDePattern("if", "3.txt")).build()}},
            {"12", {"w",  AstBuilder(TestDesignExtractorUtils::readDePattern("if", "12.txt")).build()}},
            {"13", {"ww", AstBuilder(TestDesignExtractorUtils::readDePattern("if", "13.txt")).build()}},
            {"15", {"r",  AstBuilder(TestDesignExtractorUtils::readDePattern("if", "15.txt")).build()}},
            {"16", {"rr", AstBuilder(TestDesignExtractorUtils::readDePattern("if", "16.txt")).build()}},
            {"18", {"t",  AstBuilder(TestDesignExtractorUtils::readDePattern("if", "18.txt")).build()}},
            {"19", {"tt", AstBuilder(TestDesignExtractorUtils::readDePattern("if", "19.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
    unordered_map<string, unordered_set<string>> expectedIfPattern = {
            {"1",  {"a"}},
            {"4",  {"b"}},
            {"7",  {"c", "d"}},
            {"10", {"e"}},
            {"11", {"f"}},
            {"14", {"i"}},
            {"17", {"g", "h"}}
    };
    REQUIRE(pe.getIfPatternMap() == expectedIfPattern);
    REQUIRE(pe.getWhilePatternMap().empty());
    for (auto[stmt, expectedPair]: expectedAssignPattern)
        delete expectedPair.second;
    delete ast;
}

TEST_CASE("PatternExtractor: Non-nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("non_nested.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"2", {"b", AstBuilder(TestDesignExtractorUtils::readDePattern("non-nested", "2.txt")).build()}},
            {"4", {"e", AstBuilder(TestDesignExtractorUtils::readDePattern("non-nested", "4.txt")).build()}},
            {"5", {"f", AstBuilder(TestDesignExtractorUtils::readDePattern("non-nested", "5.txt")).build()}},
            {"7", {"j", AstBuilder(TestDesignExtractorUtils::readDePattern("non-nested", "7.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
    unordered_map<string, unordered_set<string>> expectedIfPattern = {{"6", {"g", "h", "i"}}};
    REQUIRE(pe.getIfPatternMap() == expectedIfPattern);
    unordered_map<string, unordered_set<string>> expectedWhilePattern = {{"3", {"d"}}};
    REQUIRE(pe.getWhilePatternMap() == expectedWhilePattern);
    for (auto[stmt, expectedPair]: expectedAssignPattern)
        delete expectedPair.second;
    delete ast;
}

TEST_CASE("PatternExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("nested.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"4", {"d", AstBuilder(TestDesignExtractorUtils::readDePattern("nested", "4.txt")).build()}},
            {"6", {"i", AstBuilder(TestDesignExtractorUtils::readDePattern("nested", "6.txt")).build()}},
            {"8", {"n", AstBuilder(TestDesignExtractorUtils::readDePattern("nested", "8.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
    unordered_map<string, unordered_set<string>> expectedIfPattern = {{"3", {"c"}}};
    REQUIRE(pe.getIfPatternMap() == expectedIfPattern);
    unordered_map<string, unordered_set<string>> expectedWhilePattern = {{"1", {"a"}},
                                                                         {"7", {"j", "k", "m"}}};
    REQUIRE(pe.getWhilePatternMap() == expectedWhilePattern);
    for (auto[stmt, expectedPair]: expectedAssignPattern)
        delete expectedPair.second;
    delete ast;
}

TEST_CASE("PatternExtractor: n3iif") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3iif.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"2",  {"if",    AstBuilder(TestDesignExtractorUtils::readDePattern("n3iif", "2.txt")).build()}},
            {"3",  {"if",    AstBuilder(TestDesignExtractorUtils::readDePattern("n3iif", "3.txt")).build()}},
            {"9",  {"read",  AstBuilder(TestDesignExtractorUtils::readDePattern("n3iif", "9.txt")).build()}},
            {"10", {"print", AstBuilder(TestDesignExtractorUtils::readDePattern("n3iif", "10.txt")).build()}},
            {"11", {"if",    AstBuilder(TestDesignExtractorUtils::readDePattern("n3iif", "11.txt")).build()}},
            {"14", {"else",  AstBuilder(TestDesignExtractorUtils::readDePattern("n3iif", "14.txt")).build()}},
            {"15", {"then",  AstBuilder(TestDesignExtractorUtils::readDePattern("n3iif", "15.txt")).build()}},
            {"16", {"cream", AstBuilder(TestDesignExtractorUtils::readDePattern("n3iif", "16.txt")).build()}},
            {"17", {"if",    AstBuilder(TestDesignExtractorUtils::readDePattern("n3iif", "17.txt")).build()}},
            {"18", {"if",    AstBuilder(TestDesignExtractorUtils::readDePattern("n3iif", "18.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
    unordered_map<string, unordered_set<string>> expectedIfPattern = {
            {"1", {"if", "else", "if", "call", "read", "print", "while"}},
            {"6", {"abe"}}
    };
    REQUIRE(pe.getIfPatternMap() == expectedIfPattern);
    REQUIRE(pe.getWhilePatternMap().empty());
    for (auto[stmt, expectedPair]: expectedAssignPattern)
        delete expectedPair.second;
    delete ast;
}

TEST_CASE("PatternExtractor: n3iwl") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3iwl.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"1",  {"procedure", AstBuilder(TestDesignExtractorUtils::readDePattern("n3iwl", "1.txt")).build()}},
            {"3",  {"print",     AstBuilder(TestDesignExtractorUtils::readDePattern("n3iwl", "3.txt")).build()}},
            {"9",  {"try",       AstBuilder(TestDesignExtractorUtils::readDePattern("n3iwl", "9.txt")).build()}},
            {"12", {"while",     AstBuilder(TestDesignExtractorUtils::readDePattern("n3iwl", "12.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
    unordered_map<string, unordered_set<string>> expectedIfPattern = {
            {"2", {"a", "b", "c", "if", "else", "then"}}
    };
    REQUIRE(pe.getIfPatternMap() == expectedIfPattern);
    unordered_map<string, unordered_set<string>> expectedWhilePattern = {
            {"4",  {"and", "two"}},
            {"8",  {"bob", "builder"}},
            {"11", {"while"}}
    };
    REQUIRE(pe.getWhilePatternMap() == expectedWhilePattern);
    for (auto[stmt, expectedPair]: expectedAssignPattern)
        delete expectedPair.second;
    delete ast;
}

TEST_CASE("PatternExtractor: n3wim") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3wim.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"1",  {"a", AstBuilder(TestDesignExtractorUtils::readDePattern("n3wim", "1.txt")).build()}},
            {"3",  {"b", AstBuilder(TestDesignExtractorUtils::readDePattern("n3wim", "3.txt")).build()}},
            {"5",  {"c", AstBuilder(TestDesignExtractorUtils::readDePattern("n3wim", "5.txt")).build()}},
            {"21", {"c", AstBuilder(TestDesignExtractorUtils::readDePattern("n3wim", "21.txt")).build()}},
            {"22", {"d", AstBuilder(TestDesignExtractorUtils::readDePattern("n3wim", "22.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
    unordered_map<string, unordered_set<string>> expectedIfPattern = {
            {"4",  {"a", "b"}},
            {"8",  {"d", "c", "a", "b"}},
            {"16", {"a"}}
    };
    REQUIRE(pe.getIfPatternMap() == expectedIfPattern);
    unordered_map<string, unordered_set<string>> expectedWhilePattern = {
            {"2",  {"a", "b", "c", "d"}},
            {"6",  {"a"}},
            {"14", {"a", "b", "c", "d"}}
    };
    REQUIRE(pe.getWhilePatternMap() == expectedWhilePattern);
    for (auto[stmt, expectedPair]: expectedAssignPattern)
        delete expectedPair.second;
    delete ast;
}

TEST_CASE("PatternExtractor: n3wwl") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3wwl.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"1", {"while", AstBuilder(TestDesignExtractorUtils::readDePattern("n3wwl", "1.txt")).build()}},
            {"3", {"while", AstBuilder(TestDesignExtractorUtils::readDePattern("n3wwl", "3.txt")).build()}},
            {"5", {"while", AstBuilder(TestDesignExtractorUtils::readDePattern("n3wwl", "5.txt")).build()}},
            {"7", {"while", AstBuilder(TestDesignExtractorUtils::readDePattern("n3wwl", "7.txt")).build()}},
            {"9", {"while", AstBuilder(TestDesignExtractorUtils::readDePattern("n3wwl", "9.txt")).build()}}
    };
    REQUIRE(TestDesignExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
    REQUIRE(pe.getIfPatternMap().empty());
    unordered_map<string, unordered_set<string>> expectedWhilePattern = {
            {"2", {"if", "while"}},
            {"4", {"e2", "a", "b", "c"}}
    };
    REQUIRE(pe.getWhilePatternMap() == expectedWhilePattern);
    for (auto[stmt, expectedPair]: expectedAssignPattern)
        delete expectedPair.second;
    delete ast;
}
