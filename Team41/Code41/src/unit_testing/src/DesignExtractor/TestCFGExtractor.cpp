#include "catch.hpp"

#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/CFGExtractor.h"
#include "Common/AstBuilder.h"
#include "Common/CFGBuilder.h"
#include "Common/CFGUtils.h"
#include "TestExtractorUtils.h"

using namespace std;

/**
 * Assign, Read, Print, While, If, Non-nested, Nested, n3iif, n3iwl, n3wim, n3wwl
 * multiproc1, multiproc2, multiproc3, multiproc4
 */

TEST_CASE("CFGExtractor: Assign") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("assign.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1", {"2"}},
            {"2", {"3"}},
            {"3", {"4"}},
            {"4", {"5"}},
            {"5", {"6"}},
            {"6", {"7"}},
            {"7", {"8"}},
            {"8", {}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}

TEST_CASE("CFGExtractor: Read") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("read.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1", {"2"}},
            {"2", {"3"}},
            {"3", {}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}

TEST_CASE("CFGExtractor: Print") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("print.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1", {"2"}},
            {"2", {"3"}},
            {"3", {}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}

TEST_CASE("CFGExtractor: While") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("while.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1",  {"2", "3"}},
            {"2",  {"1"}},
            {"3",  {"4", "5"}},
            {"4",  {"3"}},
            {"5",  {"6", "7"}},
            {"6",  {"5"}},
            {"7",  {"8", "10"}},
            {"8",  {"9", "7"}},
            {"9",  {"8"}},
            {"10", {"11"}},
            {"11", {"10"}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}

TEST_CASE("CFGExtractor: If") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("if.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1",  {"2",  "3"}},
            {"2",  {"4"}},
            {"3",  {"4"}},
            {"4",  {"5",  "6"}},
            {"5",  {"7"}},
            {"6",  {"7"}},
            {"7",  {"8",  "9"}},
            {"8",  {"10"}},
            {"9",  {"10"}},
            {"10", {"11", "14"}},
            {"11", {"12", "13"}},
            {"12", {"17"}},
            {"13", {"17"}},
            {"14", {"15", "16"}},
            {"15", {"17"}},
            {"16", {"17"}},
            {"17", {"18", "19"}},
            {"18", {}},
            {"19", {}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}

TEST_CASE("CFGExtractor: Non-nested") {
    // non_nested-simple.txt
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("non_nested.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1",  {"2"}},
            {"2",  {"3"}},
            {"3",  {"4", "6"}},
            {"4",  {"5"}},
            {"5",  {"3"}},
            {"6",  {"7", "8"}},
            {"7",  {"10"}},
            {"8",  {"9"}},
            {"9",  {"10"}},
            {"10", {}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}

TEST_CASE("CFGExtractor: Nested") {
    // nested-simple.txt
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("nested.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1",  {"2"}},
            {"2",  {"3"}},
            {"3",  {"4", "6"}},
            {"4",  {"5"}},
            {"5",  {"1"}},
            {"6",  {"7"}},
            {"7",  {"8", "1"}},
            {"8",  {"9"}},
            {"9",  {"10"}},
            {"10", {"7"}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}

TEST_CASE("CFGExtractor: n3iif") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3iif.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1",  {"2",  "4"}},
            {"2",  {"3"}},
            {"3",  {"17"}},
            {"4",  {"5",  "13"}},
            {"5",  {"6",  "10"}},
            {"6",  {"7",  "8"}},
            {"7",  {"9"}},
            {"8",  {"9"}},
            {"9",  {"11"}},
            {"10", {"11"}},
            {"11", {"12"}},
            {"12", {"17"}},
            {"13", {"14", "15"}},
            {"14", {"16"}},
            {"15", {"16"}},
            {"16", {"17"}},
            {"17", {"18"}},
            {"18", {}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}

TEST_CASE("CFGExtractor: n3iwl") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3iwl.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1",  {"2"}},
            {"2",  {"3",  "13"}},
            {"3",  {"4"}},
            {"4",  {"5"}},
            {"5",  {"6"}},
            {"6",  {"7",  "10"}},
            {"7",  {"8"}},
            {"8",  {"9",  "4"}},
            {"9",  {"8"}},
            {"10", {"11"}},
            {"11", {"12", "4"}},
            {"12", {"11"}},
            {"13", {}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}

TEST_CASE("CFGExtractor: n3wim") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3wim.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1",  {"2"}},
            {"2",  {"3",  "22"}},
            {"3",  {"4"}},
            {"4",  {"5",  "13"}},
            {"5",  {"6"}},
            {"6",  {"7",  "12"}},
            {"7",  {"8"}},
            {"8",  {"9",  "10"}},
            {"9",  {"11"}},
            {"10", {"11"}},
            {"11", {"6"}},
            {"12", {"21"}},
            {"13", {"14"}},
            {"14", {"15", "20"}},
            {"15", {"16"}},
            {"16", {"17", "18"}},
            {"17", {"19"}},
            {"18", {"19"}},
            {"19", {"14"}},
            {"20", {"21"}},
            {"21", {"2"}},
            {"22", {}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}

TEST_CASE("CFGExtractor: n3wwl") {
    TNode *ast = AstBuilder(TestExtractorUtils::readSimpleProgram("n3wwl.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1", {"2"}},
            {"2", {"3"}},
            {"3", {"4"}},
            {"4", {"5", "2"}},
            {"5", {"6"}},
            {"6", {"7", "4"}},
            {"7", {"8"}},
            {"8", {"9", "6"}},
            {"9", {"8"}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}

TEST_CASE("CFGExtractor: multiproc1") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("multiproc/multiproc1.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1",  {"2",  "6"}},
            {"2",  {"3",  "4"}},
            {"3",  {"5"}},
            {"4",  {"5"}},
            {"5",  {"8"}},
            {"6",  {"7"}},
            {"7",  {"8"}},
            {"8",  {"9"}},
            {"9",  {}},
            {"10", {"11", "13"}},
            {"11", {"12"}},
            {"12", {"10"}},
            {"13", {"14"}},
            {"14", {}},
            {"15", {"16"}},
            {"16", {"17", "19"}},
            {"17", {"18"}},
            {"18", {"16"}},
            {"19", {}},
            {"20", {"21"}},
            {"21", {"22"}},
            {"22", {"23"}},
            {"23", {"22"}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1", "10", "15", "20"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}

TEST_CASE("CFGExtractor: multiproc2") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("multiproc/multiproc2.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1",  {"2",  "8"}},
            {"2",  {"3",  "5"}},
            {"3",  {"4"}},
            {"4",  {"7"}},
            {"5",  {"6"}},
            {"6",  {"7"}},
            {"7",  {"9"}},
            {"8",  {"9"}},
            {"9",  {"10"}},
            {"10", {}},
            {"11", {"12", "13"}},
            {"12", {"11"}},
            {"13", {"14"}},
            {"14", {"15"}},
            {"15", {}},
            {"16", {"17"}},
            {"17", {"18", "20"}},
            {"18", {"19"}},
            {"19", {"17"}},
            {"20", {}},
            {"21", {"22"}},
            {"22", {"23"}},
            {"23", {"24"}},
            {"24", {"23"}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1", "11", "16", "21"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}

TEST_CASE("CFGExtractor: multiproc3") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("multiproc/multiproc3.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1",  {"2",  "9"}},
            {"2",  {"3",  "4"}},
            {"3",  {"5"}},
            {"4",  {"5"}},
            {"5",  {"6"}},
            {"6",  {"7"}},
            {"7",  {"8"}},
            {"8",  {"10"}},
            {"9",  {"10"}},
            {"10", {"11"}},
            {"11", {}},
            {"12", {"13", "14"}},
            {"13", {"12"}},
            {"14", {"15"}},
            {"15", {}},
            {"16", {"17"}},
            {"17", {"18", "19"}},
            {"18", {"17"}},
            {"19", {}},
            {"20", {"21"}},
            {"21", {"22"}},
            {"22", {"23"}},
            {"23", {"22"}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1", "12", "16", "20"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}

TEST_CASE("CFGExtractor: multiproc4") {
    TNode *ast = AstBuilder(TestExtractorUtils::readDeInput("multiproc/multiproc4.x")).build();
    unordered_map<TNode *, string> nodeToStmtNumMap = TestExtractorUtils::makeNodeToStmtNumMap(ast);
    CFGExtractor cfge = CFGExtractor(ast, nodeToStmtNumMap);
    cfge.extract();

    unordered_map<string, vector<string>> stmtNextMap = {
            {"1",  {"2",  "7"}},
            {"2",  {"3",  "4"}},
            {"3",  {"6"}},
            {"4",  {"5"}},
            {"5",  {"6"}},
            {"6",  {"8"}},
            {"7",  {"8"}},
            {"8",  {"9"}},
            {"9",  {}},
            {"10", {"11", "12"}},
            {"11", {"10"}},
            {"12", {"13"}},
            {"13", {}},
            {"14", {"15"}},
            {"15", {"16"}},
            {"16", {"17", "18"}},
            {"17", {"16"}},
            {"18", {}},
            {"19", {"20"}},
            {"20", {"21"}},
            {"21", {"22"}},
            {"22", {"21"}}
    };
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, {"1", "10", "14", "19"});
    cfgBuilder.build();

    REQUIRE(CFGUtils::isEqual(cfge.getCFG(), cfgBuilder.getCFG()));
    delete ast;
}
