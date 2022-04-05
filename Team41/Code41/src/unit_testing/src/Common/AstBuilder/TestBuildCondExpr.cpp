#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"
#include "../AstNode/AstUtils.h"

using namespace std;

TEST_CASE("AST Builder: conditional expression") {
    SECTION("1") {
        // 0 > 0
        string simple = TestAstBuilderUtils::readFile("condexpr", "1-simple.txt");
        string xml = TestAstBuilderUtils::readFile("condexpr", "1-xml.txt");
        // manually build actual AST
        TNode *gt = AstUtils::makeGt(
                AstUtils::makeConstVal(Token::makeConst("0")),
                AstUtils::makeConstVal(Token::makeConst("0"))
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
        TNode *ge = AstUtils::makeGe(
                AstUtils::makeConstVal(Token::makeConst("1")),
                AstUtils::makeConstVal(Token::makeConst("1"))
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
        TNode *lt = AstUtils::makeLt(
                AstUtils::makeConstVal(Token::makeConst("2")),
                AstUtils::makeConstVal(Token::makeConst("2"))
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
        TNode *le = AstUtils::makeLe(
                AstUtils::makeConstVal(Token::makeConst("3")),
                AstUtils::makeConstVal(Token::makeConst("3"))
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
        TNode *eq = AstUtils::makeEq(
                AstUtils::makeConstVal(Token::makeConst("4")),
                AstUtils::makeConstVal(Token::makeConst("4"))
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
        TNode *ne = AstUtils::makeNe(
                AstUtils::makeConstVal(Token::makeConst("5")),
                AstUtils::makeConstVal(Token::makeConst("5"))
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
        TNode *nt = AstUtils::makeNot(
                AstUtils::makeEq(
                        AstUtils::makeVarName(Token::makeName("a")),
                        AstUtils::makeVarName(Token::makeName("a"))
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
        TNode *ad = AstUtils::makeAnd(
                AstUtils::makeEq(
                        AstUtils::makeVarName(Token::makeName("b")),
                        AstUtils::makeVarName(Token::makeName("b"))
                ),
                AstUtils::makeEq(
                        AstUtils::makeVarName(Token::makeName("c")),
                        AstUtils::makeVarName(Token::makeName("c"))
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
        TNode *o = AstUtils::makeOr(
                AstUtils::makeEq(
                        AstUtils::makeVarName(Token::makeName("d")),
                        AstUtils::makeConstVal(Token::makeConst("11"))
                ),
                AstUtils::makeEq(
                        AstUtils::makeVarName(Token::makeName("e")),
                        AstUtils::makeConstVal(Token::makeConst("10"))
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
        TNode *gt = AstUtils::makeGt(
                AstUtils::makePlus(
                        AstUtils::makeConstVal(Token::makeConst("1")),
                        AstUtils::makeTimes(
                                AstUtils::makeConstVal(Token::makeConst("2")),
                                AstUtils::makeConstVal(Token::makeConst("3"))
                        )
                ),
                AstUtils::makeMinus(
                        AstUtils::makeDiv(
                                AstUtils::makeConstVal(Token::makeConst("1")),
                                AstUtils::makeConstVal(Token::makeConst("2"))
                        ),
                        AstUtils::makeConstVal(Token::makeConst("3"))
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
