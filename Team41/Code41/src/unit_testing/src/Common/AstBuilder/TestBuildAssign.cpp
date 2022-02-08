#include "catch.hpp"
#include "SimpleParser/Parser.h"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

using namespace std;

TEST_CASE("AST Builder: assign") {
    SECTION("x = 0;") {
        string xml = "<assign>\n"
                     "\t<var name=x></var>\n"
                     "\t<const val=0></const>\n"
                     "</assign>";
        // manually build actual AST
        Token* var = Token::makeVar("x");
        Token* integer = Token::makeConst("0");
        TNode* assign = TNode::makeAssignStmt(
                TNode::makeVarName(var),
                TNode::makeConstVal(integer));
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, assign));
    }
}
