#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"
#include "../AstNode/AstUtils.h"

using namespace std;

TEST_CASE("AST Builder: while statement") {
    SECTION("1") {
        // while (1==1) { print x; }
        string simple = TestAstBuilderUtils::readFile("while", "1-simple.txt");
        string xml = TestAstBuilderUtils::readFile("while", "1-xml.txt");
        // manually build actual AST
        TNode *whl = AstUtils::makeWhileStmt(
                AstUtils::makeEq(
                        AstUtils::makeConstVal(Token::makeConst("1")),
                        AstUtils::makeConstVal(Token::makeConst("1"))
                ),
                AstUtils::makeStmtLst({AstUtils::makePrintStmt(AstUtils::makeVarName(Token::makeName("x")))})
        );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, whl));
        delete ast;
        delete whl;
    }
}
