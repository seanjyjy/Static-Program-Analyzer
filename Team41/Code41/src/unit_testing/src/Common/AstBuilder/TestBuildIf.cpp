#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

using namespace std;

TEST_CASE("AST Builder: if statement") {
    SECTION("1") {
        // if (1==1) then { print x; } else { print y; }
        string simple = TestAstBuilderUtils::readFile("if", "1-simple.txt");
        string xml = TestAstBuilderUtils::readFile("if", "1-xml.txt");
        // manually build actual AST
        TNode *node = TNode::makeIfStmt(
                TNode::makeEq(
                    TNode::makeConstVal(Token::makeConst("1")),
                    TNode::makeConstVal(Token::makeConst("1"))
                ),
                TNode::makeStmtLst({
                    TNode::makePrintStmt(TNode::makeVarName(Token::makeName("x")))
                }),
                TNode::makeStmtLst({
                   TNode::makePrintStmt(TNode::makeVarName(Token::makeName("y")))
                })
                );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, node));
        delete ast;
        delete node;
    }
}
