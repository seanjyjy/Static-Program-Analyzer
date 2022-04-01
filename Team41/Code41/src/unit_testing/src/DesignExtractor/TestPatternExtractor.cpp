#include "catch.hpp"

#include "DesignExtractor/PatternExtractor.h"
#include "Common/AstBuilder.h"
#include "TestExtractorUtils.h"

using namespace std;

/**
 * Assign, Read, Print, While, If, Non-nested, Nested, n3iif, n3iwl, n3wim, n3wwl
 */

TEST_CASE("PatternExtractor: Assign") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("assign.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"1", {"a", AstBuilder(TestExtractorUtils::readDePattern("assign", "1.txt")).build()}},
            {"2", {"b", AstBuilder(TestExtractorUtils::readDePattern("assign", "2.txt")).build()}},
            {"3", {"d", AstBuilder(TestExtractorUtils::readDePattern("assign", "3.txt")).build()}},
            {"4", {"e", AstBuilder(TestExtractorUtils::readDePattern("assign", "4.txt")).build()}},
            {"5", {"g", AstBuilder(TestExtractorUtils::readDePattern("assign", "5.txt")).build()}},
            {"6", {"i", AstBuilder(TestExtractorUtils::readDePattern("assign", "6.txt")).build()}},
            {"7", {"m", AstBuilder(TestExtractorUtils::readDePattern("assign", "7.txt")).build()}},
            {"8", {"q", AstBuilder(TestExtractorUtils::readDePattern("assign", "8.txt")).build()}}
    };
    REQUIRE(TestExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
    REQUIRE(pe.getIfPatternMap().empty());
    REQUIRE(pe.getWhilePatternMap().empty());
    for (auto[stmt, expectedPair]: expectedAssignPattern)
        delete expectedPair.second;
    delete ast;
}

TEST_CASE("PatternExtractor: Read") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("read.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    REQUIRE(pe.getAssignPatternMap().empty());
    REQUIRE(pe.getIfPatternMap().empty());
    REQUIRE(pe.getWhilePatternMap().empty());
    delete ast;
}

TEST_CASE("PatternExtractor: Print") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("print.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    REQUIRE(pe.getAssignPatternMap().empty());
    REQUIRE(pe.getIfPatternMap().empty());
    REQUIRE(pe.getWhilePatternMap().empty());
    delete ast;
}

TEST_CASE("PatternExtractor: While") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("while.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"2",  {"x", AstBuilder(TestExtractorUtils::readDePattern("while", "2.txt")).build()}},
            {"9",  {"w", AstBuilder(TestExtractorUtils::readDePattern("while", "9.txt")).build()}},
            {"11", {"t", AstBuilder(TestExtractorUtils::readDePattern("while", "11.txt")).build()}}
    };
    REQUIRE(TestExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
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
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("if.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"2",  {"x",  AstBuilder(TestExtractorUtils::readDePattern("if", "2.txt")).build()}},
            {"3",  {"xx", AstBuilder(TestExtractorUtils::readDePattern("if", "3.txt")).build()}},
            {"12", {"w",  AstBuilder(TestExtractorUtils::readDePattern("if", "12.txt")).build()}},
            {"13", {"ww", AstBuilder(TestExtractorUtils::readDePattern("if", "13.txt")).build()}},
            {"15", {"r",  AstBuilder(TestExtractorUtils::readDePattern("if", "15.txt")).build()}},
            {"16", {"rr", AstBuilder(TestExtractorUtils::readDePattern("if", "16.txt")).build()}},
            {"18", {"t",  AstBuilder(TestExtractorUtils::readDePattern("if", "18.txt")).build()}},
            {"19", {"tt", AstBuilder(TestExtractorUtils::readDePattern("if", "19.txt")).build()}}
    };
    REQUIRE(TestExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
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
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("non_nested.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"2", {"b", AstBuilder(TestExtractorUtils::readDePattern("non-nested", "2.txt")).build()}},
            {"4", {"e", AstBuilder(TestExtractorUtils::readDePattern("non-nested", "4.txt")).build()}},
            {"5", {"f", AstBuilder(TestExtractorUtils::readDePattern("non-nested", "5.txt")).build()}},
            {"7", {"j", AstBuilder(TestExtractorUtils::readDePattern("non-nested", "7.txt")).build()}}
    };
    REQUIRE(TestExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
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
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("nested.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"4", {"d", AstBuilder(TestExtractorUtils::readDePattern("nested", "4.txt")).build()}},
            {"6", {"i", AstBuilder(TestExtractorUtils::readDePattern("nested", "6.txt")).build()}},
            {"8", {"n", AstBuilder(TestExtractorUtils::readDePattern("nested", "8.txt")).build()}}
    };
    REQUIRE(TestExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
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
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3iif.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"2",  {"if",    AstBuilder(TestExtractorUtils::readDePattern("n3iif", "2.txt")).build()}},
            {"3",  {"if",    AstBuilder(TestExtractorUtils::readDePattern("n3iif", "3.txt")).build()}},
            {"9",  {"read",  AstBuilder(TestExtractorUtils::readDePattern("n3iif", "9.txt")).build()}},
            {"10", {"print", AstBuilder(TestExtractorUtils::readDePattern("n3iif", "10.txt")).build()}},
            {"11", {"if",    AstBuilder(TestExtractorUtils::readDePattern("n3iif", "11.txt")).build()}},
            {"14", {"else",  AstBuilder(TestExtractorUtils::readDePattern("n3iif", "14.txt")).build()}},
            {"15", {"then",  AstBuilder(TestExtractorUtils::readDePattern("n3iif", "15.txt")).build()}},
            {"16", {"cream", AstBuilder(TestExtractorUtils::readDePattern("n3iif", "16.txt")).build()}},
            {"17", {"if",    AstBuilder(TestExtractorUtils::readDePattern("n3iif", "17.txt")).build()}},
            {"18", {"if",    AstBuilder(TestExtractorUtils::readDePattern("n3iif", "18.txt")).build()}}
    };
    REQUIRE(TestExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
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
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3iwl.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"1",  {"procedure", AstBuilder(TestExtractorUtils::readDePattern("n3iwl", "1.txt")).build()}},
            {"3",  {"print",     AstBuilder(TestExtractorUtils::readDePattern("n3iwl", "3.txt")).build()}},
            {"9",  {"try",       AstBuilder(TestExtractorUtils::readDePattern("n3iwl", "9.txt")).build()}},
            {"12", {"while",     AstBuilder(TestExtractorUtils::readDePattern("n3iwl", "12.txt")).build()}}
    };
    REQUIRE(TestExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
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
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3wim.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"1",  {"a", AstBuilder(TestExtractorUtils::readDePattern("n3wim", "1.txt")).build()}},
            {"3",  {"b", AstBuilder(TestExtractorUtils::readDePattern("n3wim", "3.txt")).build()}},
            {"5",  {"c", AstBuilder(TestExtractorUtils::readDePattern("n3wim", "5.txt")).build()}},
            {"21", {"c", AstBuilder(TestExtractorUtils::readDePattern("n3wim", "21.txt")).build()}},
            {"22", {"d", AstBuilder(TestExtractorUtils::readDePattern("n3wim", "22.txt")).build()}}
    };
    REQUIRE(TestExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
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
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3wwl.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    PatternExtractor pe = PatternExtractor(ast, nodeToStmtNumMap);
    pe.extract();

    unordered_map<string, pair<string, TNode *>> expectedAssignPattern = {
            {"1", {"while", AstBuilder(TestExtractorUtils::readDePattern("n3wwl", "1.txt")).build()}},
            {"3", {"while", AstBuilder(TestExtractorUtils::readDePattern("n3wwl", "3.txt")).build()}},
            {"5", {"while", AstBuilder(TestExtractorUtils::readDePattern("n3wwl", "5.txt")).build()}},
            {"7", {"while", AstBuilder(TestExtractorUtils::readDePattern("n3wwl", "7.txt")).build()}},
            {"9", {"while", AstBuilder(TestExtractorUtils::readDePattern("n3wwl", "9.txt")).build()}}
    };
    REQUIRE(TestExtractorUtils::isPatternEqual(pe.getAssignPatternMap(), expectedAssignPattern));
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
