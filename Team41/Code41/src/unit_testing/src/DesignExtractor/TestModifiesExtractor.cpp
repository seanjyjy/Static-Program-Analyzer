#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/ModifiesExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("ModifiesExtractor: Assign") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "assign-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap);
    me.extractRelationship();

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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "read-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap);
    me.extractRelationship();

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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "print-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap);
    me.extractRelationship();

    REQUIRE(me.getProcModifiesMap().empty());
    REQUIRE(me.getStmtModifiesMap().empty());
    delete ast;
}

TEST_CASE("ModifiesExtractor: While") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "while-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap);
    me.extractRelationship();

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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "if-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap);
    me.extractRelationship();

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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "non_nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap);
    me.extractRelationship();

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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    ModifiesExtractor me = ModifiesExtractor(ast, nodeToStmtNumMap);
    me.extractRelationship();

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
