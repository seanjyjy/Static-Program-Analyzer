#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

using namespace std;

TEST_CASE("AST Builder: conditional expression") {
    SECTION("1") {
        // 0 > 0
        string simple = TestAstBuilderUtils::readFile("condexpr", "1-simple.txt");
        string xml = TestAstBuilderUtils::readFile("condexpr", "1-xml.txt");
        // manually build actual AST
        TNode *gt = TNode::makeGt(
                TNode::makeConstVal(Token::makeConst("0")),
                TNode::makeConstVal(Token::makeConst("0"))
        );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, gt));

        delete ast;
        delete gt;
    }

    SECTION("2") {
        // 1 >= 1
        string simple = TestAstBuilderUtils::readFile("condexpr", "2-simple.txt");
        string xml = TestAstBuilderUtils::readFile("condexpr", "2-xml.txt");
        // manually build actual AST
        TNode *ge = TNode::makeGe(
                TNode::makeConstVal(Token::makeConst("1")),
                TNode::makeConstVal(Token::makeConst("1"))
        );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, ge));
        delete ast;
        delete ge;
    }

    SECTION("3") {
        // 2 < 2
        string simple = TestAstBuilderUtils::readFile("condexpr", "3-simple.txt");
        string xml = TestAstBuilderUtils::readFile("condexpr", "3-xml.txt");
        // manually build actual AST
        TNode *lt = TNode::makeLt(
                TNode::makeConstVal(Token::makeConst("2")),
                TNode::makeConstVal(Token::makeConst("2"))
        );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, lt));
        delete ast;
        delete lt;
    }

    SECTION("4") {
        // 3 <= 3
        string simple = TestAstBuilderUtils::readFile("condexpr", "4-simple.txt");
        string xml = TestAstBuilderUtils::readFile("condexpr", "4-xml.txt");
        // manually build actual AST
        TNode *le = TNode::makeLe(
                TNode::makeConstVal(Token::makeConst("3")),
                TNode::makeConstVal(Token::makeConst("3"))
        );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, le));
        delete ast;
        delete le;
    }

    SECTION("5") {
        // 4 == 4
        string simple = TestAstBuilderUtils::readFile("condexpr", "5-simple.txt");
        string xml = TestAstBuilderUtils::readFile("condexpr", "5-xml.txt");
        // manually build actual AST
        TNode *eq = TNode::makeEq(
                TNode::makeConstVal(Token::makeConst("4")),
                TNode::makeConstVal(Token::makeConst("4"))
        );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, eq));
        delete ast;
        delete eq;
    }

    SECTION("6") {
        // 5 != 5
        string simple = TestAstBuilderUtils::readFile("condexpr", "6-simple.txt");
        string xml = TestAstBuilderUtils::readFile("condexpr", "6-xml.txt");
        // manually build actual AST
        TNode *ne = TNode::makeNe(
                TNode::makeConstVal(Token::makeConst("5")),
                TNode::makeConstVal(Token::makeConst("5"))
        );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, ne));
        delete ast;
        delete ne;
    }

    SECTION("7") {
        // !(a == a)
        string simple = TestAstBuilderUtils::readFile("condexpr", "7-simple.txt");
        string xml = TestAstBuilderUtils::readFile("condexpr", "7-xml.txt");
        // manually build actual AST
        TNode *nt = TNode::makeNot(
                TNode::makeEq(
                        TNode::makeVarName(Token::makeName("a")),
                        TNode::makeVarName(Token::makeName("a"))
                )
        );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, nt));
        delete ast;
        delete nt;
    }

    SECTION("8") {
        // (b==b)&&(c==c)
        string simple = TestAstBuilderUtils::readFile("condexpr", "8-simple.txt");
        string xml = TestAstBuilderUtils::readFile("condexpr", "8-xml.txt");
        // manually build actual AST
        TNode *ad = TNode::makeAnd(
                TNode::makeEq(
                        TNode::makeVarName(Token::makeName("b")),
                        TNode::makeVarName(Token::makeName("b"))
                ),
                TNode::makeEq(
                        TNode::makeVarName(Token::makeName("c")),
                        TNode::makeVarName(Token::makeName("c"))
                )
        );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, ad));
        delete ast;
        delete ad;
    }

    SECTION("9") {
        // (d==11)||(e==10)
        string simple = TestAstBuilderUtils::readFile("condexpr", "9-simple.txt");
        string xml = TestAstBuilderUtils::readFile("condexpr", "9-xml.txt");
        // manually build actual AST
        TNode *o = TNode::makeOr(
                TNode::makeEq(
                        TNode::makeVarName(Token::makeName("d")),
                        TNode::makeConstVal(Token::makeConst("11"))
                ),
                TNode::makeEq(
                        TNode::makeVarName(Token::makeName("e")),
                        TNode::makeConstVal(Token::makeConst("10"))
                )
        );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, o));
        delete ast;
        delete o;
    }

    SECTION("10") {
        // 1 + 2 * 3 > 1 / 2 - 3
        string simple = TestAstBuilderUtils::readFile("condexpr", "10-simple.txt");
        string xml = TestAstBuilderUtils::readFile("condexpr", "10-xml.txt");
        // manually build actual AST
        TNode *gt = TNode::makeGt(
                TNode::makePlus(
                        TNode::makeConstVal(Token::makeConst("1")),
                        TNode::makeTimes(
                                TNode::makeConstVal(Token::makeConst("2")),
                                TNode::makeConstVal(Token::makeConst("3"))
                        )
                ),
                TNode::makeMinus(
                        TNode::makeDiv(
                                TNode::makeConstVal(Token::makeConst("1")),
                                TNode::makeConstVal(Token::makeConst("2"))
                        ),
                        TNode::makeConstVal(Token::makeConst("3"))
                )
        );
        // parse and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, gt));
        delete gt;
        delete ast;
    }
}
