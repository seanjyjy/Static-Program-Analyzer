#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"
#include "../AstNode/AstUtils.h"
#include <iostream>

using namespace std;

TEST_CASE("AST Builder: statement list") {
    SECTION("1") {
        /* we need to wrap it in a procedure due to limitations on how the parser parses stmtlist
            procedure stmtlist {
                read x;
                print x;
                call main;
                x = 0;
                while (1==1) { print x; }
                if (1==1) then { print x; } else { print y; }
            }
         */
        string simple = TestAstBuilderUtils::readFile("stmtlist", "1-simple.txt");
        string xml = TestAstBuilderUtils::readFile("stmtlist", "1-xml.txt");
        // manually build actual AST
        TNode *procedure = AstUtils::makeProcedure(
                Token::makeName("stmtlist"),
                AstUtils::makeStmtLst({
                                           AstUtils::makeReadStmt(AstUtils::makeVarName(Token::makeName("x"))),
                                           AstUtils::makePrintStmt(AstUtils::makeVarName(Token::makeName("x"))),
                                           AstUtils::makeCallStmt(AstUtils::makeProcName(Token::makeName("main"))),
                                           AstUtils::makeAssignStmt(AstUtils::makeVarName(Token::makeName("x")),
                                                                 AstUtils::makeConstVal(Token::makeConst("0"))),
                                           AstUtils::makeWhileStmt(
                                                   AstUtils::makeEq(AstUtils::makeConstVal(Token::makeConst("1")),
                                                                 AstUtils::makeConstVal(Token::makeConst("1"))),
                                                   AstUtils::makeStmtLst({AstUtils::makePrintStmt(
                                                           AstUtils::makeVarName(Token::makeName("x")))})
                                           ),
                                           AstUtils::makeIfStmt(
                                                   AstUtils::makeEq(AstUtils::makeConstVal(Token::makeConst("1")),
                                                                 AstUtils::makeConstVal(Token::makeConst("1"))),
                                                   AstUtils::makeStmtLst({AstUtils::makePrintStmt(
                                                           AstUtils::makeVarName(Token::makeName("x")))}),
                                                   AstUtils::makeStmtLst({AstUtils::makePrintStmt(
                                                           AstUtils::makeVarName(Token::makeName("y")))})
                                           )
                                   })
        );

        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, procedure));
        delete ast;
        delete procedure;
    }
}
