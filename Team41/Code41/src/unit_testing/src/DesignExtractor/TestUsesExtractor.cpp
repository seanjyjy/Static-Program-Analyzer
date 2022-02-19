#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/UsesExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("UsesExtractor: Assign") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "assign-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    unordered_map<string, unordered_set<string>> expectedProcUses = {
            {"main", {"c", "f", "h", "j", "k", "n", "p", "r", "s", "t", "u"}}
    };
    REQUIRE(ue.getProcUsesMap() == expectedProcUses);
    unordered_map<string, unordered_set<string>> expectedStmtUses = {
            {"2", {"c"}}, {"4", {"f"}}, {"5", {"h"}}, {"6", {"j", "k"}}, {"7", {"n", "p"}}, {"8", {"r", "s", "t", "u"}}
    };
    REQUIRE(ue.getStmtUsesMap() == expectedStmtUses);
    delete ast;
}

TEST_CASE("UsesExtractor: Read") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "read-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    REQUIRE(ue.getProcUsesMap().empty());
    REQUIRE(ue.getStmtUsesMap().empty());
    delete ast;
}

TEST_CASE("UsesExtractor: Print") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "print-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    unordered_map<string, unordered_set<string>> expectedProcUses = {
            {"main", {"a", "bcd", "e1fg5"}}
    };
    REQUIRE(ue.getProcUsesMap() == expectedProcUses);
    unordered_map<string, unordered_set<string>> expectedStmtUses = {
            {"1", {"a"}}, {"2", {"bcd"}}, {"3", {"e1fg5"}}
    };
    REQUIRE(ue.getStmtUsesMap() == expectedStmtUses);
    delete ast;
}

TEST_CASE("UsesExtractor: While") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "while-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    unordered_map<string, unordered_set<string>> expectedProcUses = {
            {"main", {"a", "b", "c", "d", "z", "e", "f", "v", "g", "h", "u"}}
    };
    REQUIRE(ue.getProcUsesMap() == expectedProcUses);
    unordered_map<string, unordered_set<string>> expectedStmtUses = {
            {"1", {"a"}}, {"3", {"b"}}, {"5", {"c", "d", "z"}}, {"6", {"z"}},
            {"7", {"e", "f", "v"}}, {"8", {"f", "v"}}, {"9", {"v"}},
            {"10", {"g", "h", "u"}}, {"11", {"u"}}
    };
    REQUIRE(ue.getStmtUsesMap() == expectedStmtUses);
    delete ast;
}

TEST_CASE("UsesExtractor: If") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "if-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    unordered_map<string, unordered_set<string>> expectedProcUses = {
            {"main", {"a", "b", "c", "d", "z", "zz", "e", "f", "v", "vv", "i", "s", "ss", "g", "h", "u", "uu"}}
    };
    REQUIRE(ue.getProcUsesMap() == expectedProcUses);
    unordered_map<string, unordered_set<string>> expectedStmtUses = {
            {"1", {"a"}}, {"4", {"b"}}, {"7", {"c", "d", "z" ,"zz"}}, {"8", {"z"}}, {"9", {"zz"}},
            {"10", {"e", "f", "v", "vv", "i", "s", "ss"}}, {"11", {"f", "v", "vv"}}, {"12", {"v"}}, {"13", {"vv"}},
            {"14", {"i", "s", "ss"}}, {"15", {"s"}}, {"16", {"ss"}},
            {"17", {"g", "h", "u", "uu"}}, {"18", {"u"}}, {"19", {"uu"}}
    };
    REQUIRE(ue.getStmtUsesMap() == expectedStmtUses);
    delete ast;
}

TEST_CASE("UsesExtractor: Non-nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "non_nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    unordered_map<string, unordered_set<string>> expectedProcUses = {
            {"main", {"c", "d", "g", "h", "i", "k", "n"}}
    };
    REQUIRE(ue.getProcUsesMap() == expectedProcUses);

    unordered_map<string, unordered_set<string>> expectedStmtUses = {
            {"2", {"c"}}, {"3", {"d"}}, {"6", {"g", "h", "i", "k"}}, {"8", {"k"}}, {"10", {"n"}}
    };
    REQUIRE(ue.getStmtUsesMap() == expectedStmtUses);
    delete ast;
}

TEST_CASE("UsesExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readFile("", "nested-xml.txt")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    unordered_map<string, unordered_set<string>> expectedProcUses = {
            {"main", {"a", "c", "e", "f", "g", "h", "j", "k", "m", "p", "q", "r", "t"}}
    };
    REQUIRE(ue.getProcUsesMap() == expectedProcUses);

    unordered_map<string, unordered_set<string>> expectedStmtUses = {
            {"1", {"a", "c", "e", "f", "g", "h", "j", "k", "m", "p", "q", "r", "t"}},
            {"3", {"c", "e", "f", "g", "h", "j", "k", "m", "p", "q", "r", "t"}},
            {"4", {"e", "f", "g"}}, {"5", {"h"}},
            {"7", {"j", "k", "m", "p", "q", "r", "t"}},
            {"8", {"p", "q", "r"}}, {"10", {"t"}}
    };
    REQUIRE(ue.getStmtUsesMap() == expectedStmtUses);
    delete ast;
}

