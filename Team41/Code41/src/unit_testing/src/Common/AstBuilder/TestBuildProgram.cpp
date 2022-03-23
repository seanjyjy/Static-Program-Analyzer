#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

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
        TNode *prog = TNode::makeProgram({
                                                 TNode::makeProcedure(
                                                         Token::makeName("first"),
                                                         TNode::makeStmtLst({TNode::makePrintStmt(
                                                                 TNode::makeVarName(Token::makeName("x")))})
                                                 ),
                                                 TNode::makeProcedure(
                                                         Token::makeName("second"),
                                                         TNode::makeStmtLst({TNode::makePrintStmt(
                                                                 TNode::makeVarName(Token::makeName("x")))})
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
