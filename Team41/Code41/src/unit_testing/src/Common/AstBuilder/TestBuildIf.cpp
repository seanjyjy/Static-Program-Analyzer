#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"
#include "../AstNode/AstUtils.h"

using namespace std;

TEST_CASE("AST Builder: if statement") {
    SECTION("1") {
        // if (1==1) then { print x; } else { print y; }
        string simple = TestAstBuilderUtils::readFile("if", "1-simple.txt");
        string xml = TestAstBuilderUtils::readFile("if", "1-xml.txt");
        // manually build actual AST
        TNode *node = AstUtils::makeIfStmt(
                AstUtils::makeEq(
                        AstUtils::makeConstVal(Token::makeConst("1")),
                        AstUtils::makeConstVal(Token::makeConst("1"))
                ),
                AstUtils::makeStmtLst({
                                           AstUtils::makePrintStmt(AstUtils::makeVarName(Token::makeName("x")))
                                   }),
                AstUtils::makeStmtLst({
                                           AstUtils::makePrintStmt(AstUtils::makeVarName(Token::makeName("y")))
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
