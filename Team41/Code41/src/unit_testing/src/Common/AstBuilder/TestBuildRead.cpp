#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

using namespace std;

TEST_CASE("AST Builder: read statement") {
    SECTION("read x;") {
        string xml = "<read>\n"
                     "\t<var name=x></var>\n"
                     "</read>";
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
