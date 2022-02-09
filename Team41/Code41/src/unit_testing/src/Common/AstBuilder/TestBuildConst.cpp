#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

using namespace std;

TEST_CASE("AST Builder: constant") {
    SECTION("1") {
        string simple = TestAstBuilderUtils::readFile("const", "1-simple.txt");
        string xml = TestAstBuilderUtils::readFile("const", "1-xml.txt");
        // manually build ast
        Token *integer = Token::makeConst("0");
        TNode *node = TNode::makeConstVal(integer);
        // parse xml and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, node));
    }
}
