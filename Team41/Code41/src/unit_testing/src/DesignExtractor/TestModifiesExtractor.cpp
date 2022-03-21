#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/ModifiesExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("ModifiesExtractor: Assign") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("assign.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    unordered_map<string, unordered_set<string>> callsMap; list<string> procCallOrder; // TODO: DUMMY FOR NOW
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"main", {"a", "b", "d", "e", "g", "i", "m", "q"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1", {"a"}}, {"2", {"b"}}, {"3", {"d"}}, {"4", {"e"}},
            {"5", {"g"}}, {"6", {"i"}}, {"7", {"m"}}, {"8", {"q"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: Read") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("read.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    unordered_map<string, unordered_set<string>> callsMap; list<string> procCallOrder; // TODO: DUMMY FOR NOW
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"main", {"a", "bcd", "e1fg5"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
                {"1", {"a"}}, {"2", {"bcd"}}, {"3", {"e1fg5"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: Print") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("print.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    unordered_map<string, unordered_set<string>> callsMap; list<string> procCallOrder; // TODO: DUMMY FOR NOW
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    REQUIRE(me.getProcModifiesMap().empty());
    REQUIRE(me.getStmtModifiesMap().empty());
    delete ast;
}

TEST_CASE("ModifiesExtractor: While") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("while.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    unordered_map<string, unordered_set<string>> callsMap; list<string> procCallOrder; // TODO: DUMMY FOR NOW
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"main", {"x", "y", "w", "t"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1", {"x"}}, {"2", {"x"}}, {"3", {"y"}}, {"4", {"y"}},
            {"7", {"w"}}, {"8", {"w"}}, {"9", {"w"}}, {"10", {"t"}}, {"11", {"t"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: If") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("if.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    unordered_map<string, unordered_set<string>> callsMap; list<string> procCallOrder; // TODO: DUMMY FOR NOW
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"main", {"x", "xx", "y", "yy", "w", "ww", "r", "rr", "t", "tt"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1", {"x", "xx"}}, {"2", {"x"}}, {"3", {"xx"}},
            {"4", {"y", "yy"}}, {"5", {"y"}}, {"6", {"yy"}},
            {"10", {"w", "ww", "r", "rr"}}, {"11", {"w", "ww"}}, {"12", {"w"}}, {"13", {"ww"}},
            {"14", {"r", "rr"}}, {"15", {"r"}}, {"16", {"rr"}},
            {"17", {"t", "tt"}}, {"18", {"t"}}, {"19", {"tt"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: Non-nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("non_nested.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    unordered_map<string, unordered_set<string>> callsMap; list<string> procCallOrder; // TODO: DUMMY FOR NOW
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"main", {"a", "b", "e", "f", "j", "m"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1", {"a"}}, {"2", {"b"}}, {"3", {"e", "f"}}, {"4", {"e"}}, {"5", {"f"}},
            {"6", {"j", "m"}}, {"7", {"j"}}, {"9", {"m"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("nested.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    unordered_map<string, unordered_set<string>> callsMap; list<string> procCallOrder; // TODO: DUMMY FOR NOW
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"main", {"b", "d", "i", "n", "s"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1", {"b", "d", "i", "n", "s"}}, {"2", {"b"}}, {"3", {"d", "i", "n", "s"}},
            {"4", {"d"}}, {"6", {"i"}}, {"7", {"n", "s"}}, {"8", {"n"}}, {"9", {"s"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: n3iif") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3iif.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    unordered_map<string, unordered_set<string>> callsMap; list<string> procCallOrder; // TODO: DUMMY FOR NOW
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"n3iif", {"if", "read", "print", "else", "then", "cream"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1", {"if", "read", "print", "else", "then", "cream"}}, {"2", {"if"}}, {"3", {"if"}},
            {"4", {"read", "print", "if", "else", "then", "cream"}}, {"5", {"read", "print"}},
            {"9", {"read"}}, {"10", {"print"}}, {"11", {"if"}}, {"12", {"if"}},
            {"13", {"else", "then"}}, {"14", {"else"}}, {"15", {"then"}}, {"16", {"cream"}},
            {"17", {"if"}}, {"18", {"if"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: n3iwl") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3iwl.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    unordered_map<string, unordered_set<string>> callsMap; list<string> procCallOrder; // TODO: DUMMY FOR NOW
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"n3iwl", {"procedure", "print", "read", "try", "reader", "while"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1", {"procedure"}}, {"2", {"print", "read", "try", "reader", "while"}}, {"3", {"print"}},
            {"4", {"read", "try", "reader", "while"}}, {"5", {"read"}}, {"6", {"try", "reader", "while"}},
            {"8", {"try"}}, {"9", {"try"}}, {"10", {"reader"}}, {"11", {"while"}}, {"12", {"while"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: n3wim") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3wim.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    unordered_map<string, unordered_set<string>> callsMap; list<string> procCallOrder; // TODO: DUMMY FOR NOW
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"n3wim", {"a", "b", "c", "x", "d"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1", {"a"}}, {"2", {"b", "c", "x"}}, {"3", {"b"}}, {"4", {"c", "x"}}, {"5", {"c"}},
            {"13", {"x"}}, {"14", {"x"}}, {"15", {"x"}}, {"19", {"x"}}, {"20", {"x"}},
            {"21", {"c"}}, {"22", {"d"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}

TEST_CASE("ModifiesExtractor: n3wwl") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3wwl.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extract();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    unordered_map<string, unordered_set<string>> callsMap; list<string> procCallOrder; // TODO: DUMMY FOR NOW
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap, callsMap, procCallOrder);
    me.extract();

    unordered_map<string, unordered_set<string>> expectedProcModifies = {
            {"n3wwl", {"while"}}
    };
    REQUIRE(me.getProcModifiesMap() == expectedProcModifies);
    unordered_map<string, unordered_set<string>> expectedStmtModifies = {
            {"1", {"while"}}, {"2", {"while"}}, {"3", {"while"}}, {"4", {"while"}}, {"5", {"while"}},
            {"6", {"while"}}, {"7", {"while"}}, {"8", {"while"}}, {"9", {"while"}}
    };
    REQUIRE(me.getStmtModifiesMap() == expectedStmtModifies);
    delete ast;
}
