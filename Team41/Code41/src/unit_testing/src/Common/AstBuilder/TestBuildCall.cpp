#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

using namespace std;

TEST_CASE("AST Builder: call") {
    SECTION("call main;") {
        string xml = "<call>\n"
                     "\t<var name=main></var>\n"
                     "</call>";
        // manually build actual AST
        Token* var = Token::makeVar("main");
        TNode* call = TNode::makeCallStmt(
                TNode::makeVarName(var));
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, call));
    }
}
