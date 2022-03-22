#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

using namespace std;

TEST_CASE("AST Builder: procedure") {
    SECTION("1") {
        // procedure main { print x; }
        string simple = TestAstBuilderUtils::readFile("procedure", "1-simple.txt");
        string xml = TestAstBuilderUtils::readFile("procedure", "1-xml.txt");
        // manually build actual AST
        TNode *procedure = TNode::makeProcedure(
                Token::makeName("main"),
                TNode::makeStmtLst({TNode::makePrintStmt(TNode::makeVarName(Token::makeName("x")))})
        );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, procedure));

        delete ast;
        delete procedure;
    }
}
