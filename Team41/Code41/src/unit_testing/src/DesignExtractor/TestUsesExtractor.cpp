#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/UsesExtractor.h"
#include "Common/AstBuilder.h"
#include "TestDesignExtractorUtils.h"

using namespace std;

TEST_CASE("UsesExtractor: Assign") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("assign.x")).build();
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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("read.x")).build();
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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("print.x")).build();
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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("while.x")).build();
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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("if.x")).build();
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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("non_nested.x")).build();
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
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readDeInput("nested.x")).build();
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

TEST_CASE("UsesExtractor: n3iif") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3iif.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    unordered_map<string, unordered_set<string>> expectedProcUses = {
            {"n3iif", {"if", "else", "call", "read", "print", "while", "abe", "x", "y", "then", "dollars"}}
    };
    REQUIRE(ue.getProcUsesMap() == expectedProcUses);

    unordered_map<string, unordered_set<string>> expectedStmtUses = {
            {"1", {"if", "else", "call", "read", "print", "while", "abe", "x", "y", "then", "dollars"}},
            {"2", {"if"}}, {"3", {"if"}},
            {"4", {"abe", "x", "y", "read", "print", "if", "then", "else", "dollars"}},
            {"5", {"abe", "x", "y", "read", "print"}}, {"6", {"abe", "x", "y"}}, {"7", {"x"}}, {"8", {"y"}},
            {"9", {"read"}}, {"10", {"print"}}, {"11", {"if", "then", "else"}},
            {"13", {"then", "else"}}, {"14", {"then"}}, {"15", {"else"}}, {"16", {"dollars"}}
    };
    REQUIRE(ue.getStmtUsesMap() == expectedStmtUses);
}

TEST_CASE("UsesExtractor: n3iwl") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3iwl.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    unordered_map<string, unordered_set<string>> expectedProcUses = {
            {"n3iwl", {"procedure", "a", "b", "c", "if", "else", "then", "and", "two", "print", "bob", "builder", "catch", "while", "x"}}
    };
    REQUIRE(ue.getProcUsesMap() == expectedProcUses);

    unordered_map<string, unordered_set<string>> expectedStmtUses = {
            {"1", {"procedure"}}, {"2", {"a", "b", "c", "if", "else", "then", "and", "two", "print", "bob", "builder", "catch", "while", "x"}},
            {"4", {"and", "two", "print", "bob", "builder", "catch", "while"}}, {"6", {"print", "bob", "builder", "catch", "while"}},
            {"7", {"print"}}, {"8", {"bob", "builder", "catch"}}, {"9", {"catch"}},
            {"11", {"while"}}, {"12", {"while"}}, {"13", {"x"}}
    };
    REQUIRE(ue.getStmtUsesMap() == expectedStmtUses);
}

TEST_CASE("UsesExtractor: n3wim") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3wim.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    unordered_map<string, unordered_set<string>> expectedProcUses = {
            {"n3wim", {"a", "b", "c", "d", "x", "x1", "x2", "x3", "x4"}}
    };
    REQUIRE(ue.getProcUsesMap() == expectedProcUses);

    unordered_map<string, unordered_set<string>> expectedStmtUses = {
            {"2", {"a", "b", "c", "d", "x", "x1", "x2", "x3", "x4"}}, {"3", {"a"}}, {"4", {"a", "b", "c", "x", "d", "x1", "x2", "x3", "x4"}},
            {"5", {"c", "b"}}, {"6", {"a", "x", "d", "c", "b", "x1", "x2"}}, {"7", {"x"}},
            {"8", {"d", "c", "a", "b", "x1", "x2"}}, {"9", {"x1"}}, {"10", {"x2"}}, {"11", {"x"}}, {"12", {"x"}},
            {"14", {"a", "b", "c", "d", "x3", "x4"}}, {"16", {"a", "x3", "x4"}}, {"17", {"x3"}}, {"18", {"x4"}}, {"21", {"b"}}
    };
    REQUIRE(ue.getStmtUsesMap() == expectedStmtUses);
}

TEST_CASE("UsesExtractor: n3wwl") {
    TNode *ast = AstBuilder(TestDesignExtractorUtils::readSimpleProgram("n3wwl.x")).build();
    EntitiesExtractor ee = EntitiesExtractor(ast);
    ee.extractEntities();
    unordered_map<TNode *, string> nodeToStmtNumMap = ee.getNodeToStmtNumMap();
    UsesExtractor ue = UsesExtractor(ast, nodeToStmtNumMap);
    ue.extractRelationship();

    unordered_map<string, unordered_set<string>> expectedProcUses = {
            {"n3wwl", {"while", "if", "then", "e2", "a", "b", "c", "else"}}
    };
    REQUIRE(ue.getProcUsesMap() == expectedProcUses);

    unordered_map<string, unordered_set<string>> expectedStmtUses = {
            {"1", {"while"}}, {"2", {"while", "if", "then", "e2", "a", "b", "c", "else"}}, {"3", {"then"}},
            {"4", {"e2", "a", "b", "c", "if", "else"}}, {"5", {"if"}}, {"6", {"else"}}, {"7", {"else"}}
    };
    REQUIRE(ue.getStmtUsesMap() == expectedStmtUses);
}
