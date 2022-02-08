#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"
using namespace std;

TEST_CASE("AST Builder: expression") {
    SECTION("1 + 1") {
        string xml = "<plus>\n"
                     "<const val=1></const>\n"
                     "<const val=1></const>\n"
                     "</plus>";
        // manually build actual AST
        Token* c1 = Token::makeConst("1");
        Token* c2 = Token::makeConst("1");
        TNode* plus = TNode::makePlus(
                TNode::makeConstVal(c1),
                TNode::makeConstVal(c2));
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, plus));
    }

    SECTION("a - a") {
        string xml = "<minus>\n"
                     "\t<var name=a></var>\n"
                     "\t<var name=a></var>\n"
                     "</minus>";

        TNode* minus = TNode::makeMinus(
                TNode::makeVarName(Token::makeVar("a")),
                TNode::makeVarName(Token::makeVar("a"))
                );

        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, minus));
    }

    SECTION("(1 * 1) + (2 / 2) - (a % a)") {
        string xml = "<minus>\n"
                     "<plus>\n"
                     "<times>\n"
                     "<const val=1></const>\n"
                     "<const val=1></const>\n"
                     "</times>\n"
                     "<div>\n"
                     "<const val=2></const>\n"
                     "<const val=2></const>\n"
                     "</div>\n"
                     "</plus>\n"
                     "<mod>\n"
                     "<var name=a></var>\n"
                     "<var name=a></var>\n"
                     "</mod>\n"
                     "</minus>";
        TNode* minus = TNode::makeMinus(
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
                    TNode::makeVarName(Token::makeVar("a")),
                    TNode::makeVarName(Token::makeVar("a"))
                    )
                );
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, minus));
    }
}
