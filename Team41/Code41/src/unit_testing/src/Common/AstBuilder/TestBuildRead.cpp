#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

using namespace std;

TEST_CASE("AST Builder: read statement") {
    SECTION("1") {
        // read x;
        string simple = TestAstBuilderUtils::readFile("read", "1-simple.txt");
        string xml = TestAstBuilderUtils::readFile("read", "1-xml.txt");
        // manually build actual AST
        Token* var = Token::makeVar("x");
        TNode* call = TNode::makeReadStmt(
                TNode::makeVarName(var));
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, call));
    }
}
