#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"
#include "../AstNode/AstUtils.h"

using namespace std;

TEST_CASE("AST Builder: program") {
    SECTION("1") {
        /*
            procedure first { print x; }
            procedure second { print x; }
        */
        string simple = TestAstBuilderUtils::readFile("program", "1-simple.txt");
        string xml = TestAstBuilderUtils::readFile("program", "1-xml.txt");
        // manually build actual AST
        TNode *prog = AstUtils::makeProgram({
                                                    AstUtils::makeProcedure(
                                                            Token::makeName("first"),
                                                            AstUtils::makeStmtLst({AstUtils::makePrintStmt(
                                                                    AstUtils::makeVarName(Token::makeName("x")))})
                                                 ),
                                                    AstUtils::makeProcedure(
                                                            Token::makeName("second"),
                                                            AstUtils::makeStmtLst({AstUtils::makePrintStmt(
                                                                    AstUtils::makeVarName(Token::makeName("x")))})
                                                 )
                                         });
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, prog));
        delete ast;
        delete prog;
    }
}
