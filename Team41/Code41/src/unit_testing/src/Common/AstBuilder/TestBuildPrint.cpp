#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

using namespace std;

TEST_CASE("AST Builder: print statement") {
    SECTION("print x;") {
        string xml = "<print>\n"
                     "\t<var name=x></var>\n"
                     "</print>";
        // manually build actual AST
        Token* var = Token::makeVar("x");
        TNode* call = TNode::makePrintStmt(
                TNode::makeVarName(var));
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, call));
    }
}
