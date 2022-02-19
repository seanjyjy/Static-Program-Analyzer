#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

using namespace std;

TEST_CASE("AST Builder: while statement") {
    SECTION("1") {
        // while (1==1) { print x; }
        string simple = TestAstBuilderUtils::readFile("while", "1-simple.txt");
        string xml = TestAstBuilderUtils::readFile("while", "1-xml.txt");
        // manually build actual AST
        TNode *whl = TNode::makeWhileStmt(
                TNode::makeEq(
                        TNode::makeConstVal(Token::makeConst("1")),
                        TNode::makeConstVal(Token::makeConst("1"))
                        ),
                TNode::makeStmtLst({ TNode::makePrintStmt(TNode::makeVarName(Token::makeName("x")))})
                );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, whl));
        delete ast;
        delete whl;
    }
}
