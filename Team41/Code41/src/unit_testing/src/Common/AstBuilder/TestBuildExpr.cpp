#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

using namespace std;

TEST_CASE("AST Builder: expression") {
    SECTION("1") {
        // 1 + 1
        string simple = TestAstBuilderUtils::readFile("expr", "1-simple.txt");
        string xml = TestAstBuilderUtils::readFile("expr", "1-xml.txt");
        // manually build actual AST
        Token *c1 = Token::makeConst("1");
        Token *c2 = Token::makeConst("1");
        TNode *plus = TNode::makePlus(
                TNode::makeConstVal(c1),
                TNode::makeConstVal(c2));
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, plus));

        delete ast;
        delete plus;
    }

    SECTION("2") {
        // a - a
        string simple = TestAstBuilderUtils::readFile("expr", "2-simple.txt");
        string xml = TestAstBuilderUtils::readFile("expr", "2-xml.txt");

        TNode *minus = TNode::makeMinus(
                TNode::makeVarName(Token::makeName("a")),
                TNode::makeVarName(Token::makeName("a"))
        );

        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, minus));

        delete ast;
        delete minus;
    }

    SECTION("3") {
        // (1 * 1) + (2 / 2) - (a % a)
        string simple = TestAstBuilderUtils::readFile("expr", "3-simple.txt");
        string xml = TestAstBuilderUtils::readFile("expr", "3-xml.txt");
        TNode *minus = TNode::makeMinus(
                TNode::makePlus(
                        TNode::makeTimes(
                                TNode::makeConstVal(Token::makeConst("1")),
                                TNode::makeConstVal(Token::makeConst("1"))
                        ),
                        TNode::makeDiv(
                                TNode::makeConstVal(Token::makeConst("2")),
                                TNode::makeConstVal(Token::makeConst("2"))
                        )
                ),
                TNode::makeMod(
                        TNode::makeVarName(Token::makeName("a")),
                        TNode::makeVarName(Token::makeName("a"))
                )
        );
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, minus));

        delete ast;
        delete minus;
    }
}
