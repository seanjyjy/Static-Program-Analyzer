#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

using namespace std;

TEST_CASE("AST Builder: call") {
    SECTION("1") {
        string simple = TestAstBuilderUtils::readFile("call", "1-simple.txt");
        string xml = TestAstBuilderUtils::readFile("call", "1-xml.txt");
        // manually build actual AST
        Token* name = Token::makeName("main");
        TNode* call = TNode::makeCallStmt(
                TNode::makeProcName(name));
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, call));
    }
}
