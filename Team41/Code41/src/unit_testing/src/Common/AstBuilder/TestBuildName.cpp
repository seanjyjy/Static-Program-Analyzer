#include "catch.hpp"
#include "TestAstBuilderUtils.h"
#include "Common/TreeUtils.h"

using namespace std;

TEST_CASE("AST Builder: name") {
    SECTION("'hi'") {
        string xml = "<var name=hi></var>";
        // manually build ast
        Token *var = Token::makeVar("hi");
        TNode *node = TNode::makeVarName(var);
        // parse xml and test
        TNode *ast = TestAstBuilderUtils::parseXml(xml);
        REQUIRE(ast != nullptr);
        REQUIRE(TreeUtils::isEqual(ast, node));
    }
}
