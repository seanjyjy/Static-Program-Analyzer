#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/ModifiesExtractor.h"
#include "Common/AstBuilder.h"
#include "TestExtractorUtils.h"

using namespace std;

/**
 * Assign, Read, Print, While, If, Non-nested, Nested, n3iif, n3iwl, n3wim, n3wwl
 * Call, multiproc1, multiproc2, multiproc3, multiproc4
 */

TEST_CASE("ModifiesExtractor: Assign") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("assign.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap;
    list<string> procCallOrder;
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"main", {"a", "b", "d", "e", "g", "i", "m", "q"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1", {"a"}},
            {"2", {"b"}},
            {"3", {"d"}},
            {"4", {"e"}},
            {"5", {"g"}},
            {"6", {"i"}},
            {"7", {"m"}},
            {"8", {"q"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: Read") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("read.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap;
    list<string> procCallOrder;
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"main", {"a", "bcd", "e1fg5"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1", {"a"}},
            {"2", {"bcd"}},
            {"3", {"e1fg5"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: Print") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("print.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap;
    list<string> procCallOrder;
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    REQUIRE(me.getProcModifiesMap().empty());
    REQUIRE(me.getStmtModifiesMap().empty());
    delete ast;
}

TEST_CASE("ModifiesExtractor: While") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("while.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap;
    list<string> procCallOrder;
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"main", {"x", "y", "w", "t"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1",  {"x"}},
            {"2",  {"x"}},
            {"3",  {"y"}},
            {"4",  {"y"}},
            {"7",  {"w"}},
            {"8",  {"w"}},
            {"9",  {"w"}},
            {"10", {"t"}},
            {"11", {"t"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: If") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("if.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap;
    list<string> procCallOrder;
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"main", {"x", "xx", "y", "yy", "w", "ww", "r", "rr", "t", "tt"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1",  {"x", "xx"}},
            {"2",  {"x"}},
            {"3",  {"xx"}},
            {"4",  {"y", "yy"}},
            {"5",  {"y"}},
            {"6",  {"yy"}},
            {"10", {"w", "ww", "r", "rr"}},
            {"11", {"w", "ww"}},
            {"12", {"w"}},
            {"13", {"ww"}},
            {"14", {"r", "rr"}},
            {"15", {"r"}},
            {"16", {"rr"}},
            {"17", {"t", "tt"}},
            {"18", {"t"}},
            {"19", {"tt"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: Non-nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("non_nested.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap;
    list<string> procCallOrder;
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"main", {"a", "b", "e", "f", "j", "m"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1", {"a"}},
            {"2", {"b"}},
            {"3", {"e", "f"}},
            {"4", {"e"}},
            {"5", {"f"}},
            {"6", {"j", "m"}},
            {"7", {"j"}},
            {"9", {"m"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("nested.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap;
    list<string> procCallOrder;
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"main", {"b", "d", "i", "n", "s"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1", {"b", "d", "i", "n", "s"}},
            {"2", {"b"}},
            {"3", {"d", "i", "n", "s"}},
            {"4", {"d"}},
            {"6", {"i"}},
            {"7", {"n", "s"}},
            {"8", {"n"}},
            {"9", {"s"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: n3iif") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3iif.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap;
    list<string> procCallOrder;
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"n3iif", {"if", "read", "print", "else", "then", "cream"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1",  {"if",   "read",  "print", "else", "then", "cream"}},
            {"2",  {"if"}},
            {"3",  {"if"}},
            {"4",  {"read", "print", "if",    "else", "then", "cream"}},
            {"5",  {"read", "print"}},
            {"9",  {"read"}},
            {"10", {"print"}},
            {"11", {"if"}},
            {"12", {"if"}},
            {"13", {"else", "then"}},
            {"14", {"else"}},
            {"15", {"then"}},
            {"16", {"cream"}},
            {"17", {"if"}},
            {"18", {"if"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: n3iwl") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3iwl.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap;
    list<string> procCallOrder;
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"n3iwl", {"procedure", "print", "read", "try", "reader", "while"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1",  {"procedure"}},
            {"2",  {"print", "read",   "try",    "reader", "while"}},
            {"3",  {"print"}},
            {"4",  {"read",  "try",    "reader", "while"}},
            {"5",  {"read"}},
            {"6",  {"try",   "reader", "while"}},
            {"8",  {"try"}},
            {"9",  {"try"}},
            {"10", {"reader"}},
            {"11", {"while"}},
            {"12", {"while"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: n3wim") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3wim.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap;
    list<string> procCallOrder;
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"n3wim", {"a", "b", "c", "x", "d"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1",  {"a"}},
            {"2",  {"b", "c", "x"}},
            {"3",  {"b"}},
            {"4",  {"c", "x"}},
            {"5",  {"c"}},
            {"13", {"x"}},
            {"14", {"x"}},
            {"15", {"x"}},
            {"19", {"x"}},
            {"20", {"x"}},
            {"21", {"c"}},
            {"22", {"d"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: n3wwl") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3wwl.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap;
    list<string> procCallOrder;
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"n3wwl", {"while"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1", {"while"}},
            {"2", {"while"}},
            {"3", {"while"}},
            {"4", {"while"}},
            {"5", {"while"}},
            {"6", {"while"}},
            {"7", {"while"}},
            {"8", {"while"}},
            {"9", {"while"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: Call") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("call.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap = {
            {"p1", {"p2", "p4"}},
            {"p2", {"p3"}},
            {"p4", {"p5", "p6"}},
            {"p5", {"p6"}}
    };
    list<string> procCallOrder = {"p5", "p4", "p2", "p1"};
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"p6", {"f"}},
            {"p5", {"e", "f"}},
            {"p4", {"d", "e", "f"}},
            {"p3", {"c"}},
            {"p2", {"b", "c"}},
            {"p1", {"a", "b", "c", "d", "e", "f"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1",  {"b", "c"}},
            {"2",  {"d", "e", "f"}},
            {"3",  {"a"}},
            {"4",  {"c"}},
            {"5",  {"b"}},
            {"6",  {"c"}},
            {"7",  {"d"}},
            {"8",  {"e", "f"}},
            {"9",  {"f"}},
            {"10", {"f"}},
            {"11", {"e"}},
            {"12", {"f"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: multiproc1") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("multiproc/multiproc1.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap = {
            {"n1iif", {"n0f"}},
            {"n0f",   {"n0m"}},
            {"n0m",   {"n0l"}}
    };
    list<string> procCallOrder = {"n0m", "n0f", "n1iif"};
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"n0l",   {"permanent",         "s33y0u"}},
            {"n0m",   {"temptation",        "g00dbye",        "permanent",  "s33y0u"}},
            {"n0f",   {"h3ll0",             "trader",         "temptation", "g00dbye", "permanent",  "s33y0u"}},
            {"n1iif", {"TooMuchIntoThings", "J0000000000000", "h3ll0",      "trader",  "temptation", "g00dbye", "permanent", "s33y0u"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1",  {"TooMuchIntoThings", "h3ll0",      "trader",     "temptation", "g00dbye",   "permanent", "s33y0u"}},
            {"2",  {"TooMuchIntoThings"}},
            {"3",  {"TooMuchIntoThings"}},
            {"6",  {"h3ll0",             "trader",     "temptation", "g00dbye",    "permanent", "s33y0u"}},
            {"8",  {"J0000000000000"}},
            {"10", {"h3ll0",             "temptation", "g00dbye",    "permanent",  "s33y0u"}},
            {"11", {"h3ll0"}},
            {"12", {"temptation",        "g00dbye",    "permanent",  "s33y0u"}},
            {"13", {"trader"}},
            {"15", {"temptation"}},
            {"16", {"g00dbye",           "permanent",  "s33y0u"}},
            {"17", {"permanent",         "s33y0u"}},
            {"18", {"g00dbye"}},
            {"20", {"permanent"}},
            {"22", {"s33y0u"}},
            {"23", {"s33y0u"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: multiproc2") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("multiproc/multiproc2.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap = {
            {"n1iif", {"n0f", "n0m"}},
            {"n0f",   {"n0l"}},
            {"n0m",   {"n0l"}}
    };
    list<string> procCallOrder = {"n0m", "n0f", "n1iif"};
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"n0l",   {"permanent",         "s33y0u"}},
            {"n0m",   {"temptation",        "g00dbye",        "permanent", "s33y0u"}},
            {"n0f",   {"h3ll0",             "trader",         "permanent", "s33y0u"}},
            {"n1iif", {"TooMuchIntoThings", "J0000000000000", "h3ll0",     "trader", "temptation", "g00dbye", "permanent", "s33y0u"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1",  {"TooMuchIntoThings", "h3ll0",     "trader",    "temptation", "g00dbye", "permanent", "s33y0u"}},
            {"2",  {"TooMuchIntoThings", "h3ll0",     "trader",    "temptation", "g00dbye", "permanent", "s33y0u"}},
            {"3",  {"TooMuchIntoThings"}},
            {"4",  {"h3ll0",             "trader",    "permanent", "s33y0u"}},
            {"5",  {"temptation",        "g00dbye",   "permanent", "s33y0u"}},
            {"9",  {"J0000000000000"}},
            {"11", {"h3ll0"}},
            {"12", {"h3ll0"}},
            {"13", {"permanent",         "s33y0u"}},
            {"14", {"trader"}},
            {"16", {"temptation"}},
            {"17", {"g00dbye",           "permanent", "s33y0u"}},
            {"18", {"permanent",         "s33y0u"}},
            {"19", {"g00dbye"}},
            {"21", {"permanent"}},
            {"23", {"s33y0u"}},
            {"24", {"s33y0u"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: multiproc3") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("multiproc/multiproc3.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap = {
            {"n1iif", {"n0f", "n0m", "n0l"}}
    };
    list<string> procCallOrder = {"n1iif"};
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"n0l",   {"permanent",         "s33y0u"}},
            {"n0m",   {"temptation",        "g00dbye"}},
            {"n0f",   {"h3ll0",             "trader"}},
            {"n1iif", {"TooMuchIntoThings", "J0000000000000", "h3ll0", "trader", "temptation", "g00dbye", "permanent", "s33y0u"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1",  {"TooMuchIntoThings", "h3ll0", "trader", "temptation", "g00dbye", "permanent", "s33y0u"}},
            {"2",  {"TooMuchIntoThings"}},
            {"3",  {"TooMuchIntoThings"}},
            {"6",  {"h3ll0",             "trader"}},
            {"7",  {"temptation",        "g00dbye"}},
            {"8",  {"permanent",         "s33y0u"}},
            {"10", {"J0000000000000"}},
            {"12", {"h3ll0"}},
            {"13", {"h3ll0"}},
            {"14", {"trader"}},
            {"16", {"temptation"}},
            {"17", {"g00dbye"}},
            {"18", {"g00dbye"}},
            {"20", {"permanent"}},
            {"22", {"s33y0u"}},
            {"23", {"s33y0u"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: multiproc4") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("multiproc/multiproc4.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    unordered_map<string, unordered_set<string>> callsMap = {
            {"n1iif", {"n0f"}},
            {"n0m",   {"n0l"}}
    };
    list<string> procCallOrder = {"n1iif", "n0m"};
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"n0l",   {"permanent",         "s33y0u"}},
            {"n0m",   {"temptation",        "g00dbye",        "permanent", "s33y0u"}},
            {"n0f",   {"h3ll0",             "trader"}},
            {"n1iif", {"TooMuchIntoThings", "J0000000000000", "h3ll0",     "trader"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1",  {"TooMuchIntoThings", "h3ll0", "trader"}},
            {"2",  {"TooMuchIntoThings", "h3ll0", "trader"}},
            {"3",  {"TooMuchIntoThings"}},
            {"5",  {"h3ll0",             "trader"}},
            {"8",  {"J0000000000000"}},
            {"10", {"h3ll0"}},
            {"11", {"h3ll0"}},
            {"12", {"trader"}},
            {"14", {"temptation"}},
            {"15", {"permanent",         "s33y0u"}},
            {"16", {"g00dbye"}},
            {"17", {"g00dbye"}},
            {"19", {"permanent"}},
            {"21", {"s33y0u"}},
            {"22", {"s33y0u"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}
