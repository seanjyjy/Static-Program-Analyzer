#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"
#include "../AstNode/AstUtils.h"

using namespace std;

TEST_CASE("AST Builder: name") {
    SECTION("1") {
        // 'hi'
        string simple = TestAstBuilderUtils::readFile("name", "1-simple.txt");
        string xml = TestAstBuilderUtils::readFile("name", "1-xml.txt");
        // manually build ast
        Token *var = Token::makeName("hi");
        TNode *node = AstUtils::makeVarName(var);
        // parse xml and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, node));

        delete node;
        delete ast;
    }
}
