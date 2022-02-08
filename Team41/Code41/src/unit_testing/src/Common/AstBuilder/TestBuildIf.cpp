#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

using namespace std;

TEST_CASE("AST Builder: if statement") {
    SECTION("if (1==1) then { print x; } else { print y; }") {
        string xml = "<if>\n"
                     "  <eq>\n"
                     "    <const val=1></const>\n"
                     "    <const val=1></const>\n"
                     "  </eq>"
                     "  <stmts>\n"
                     "    <print>\n"
                     "      <var name=x></var>\n"
                     "    </print>\n"
                     "  </stmts>\n"
                     "  <stmts>\n"
                     "    <print>\n"
                     "      <var name=y></var>\n"
                     "    </print>\n"
                     "  </stmts>\n"
                     "</if>";
        // manually build actual AST
        TNode *node = TNode::makeIfStmt(
                TNode::makeEq(
                    TNode::makeConstVal(Token::makeConst("1")),
                    TNode::makeConstVal(Token::makeConst("1"))
                ),
                TNode::makeStmtLst({
                    TNode::makePrintStmt(TNode::makeVarName(Token::makeVar("x")))
                }),
                TNode::makeStmtLst({
                   TNode::makePrintStmt(TNode::makeVarName(Token::makeVar("y")))
                })
                );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, node));
    }
}
