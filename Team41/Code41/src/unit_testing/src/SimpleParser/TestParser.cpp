#include <iostream>

#include "SimpleParser/Parser.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("Parser: read") {

    SECTION("1") {
        string s = "procedure main {\n"
                   "\tread x;\n"
                   "}";
        Parser p = Parser{s};
        TNode* ast = p.parse();
        ast->printRecursive();
        REQUIRE(ast != nullptr);

        Token* t = new Token(TokenType::keyword, "read", {0,0}, {0,0});
        TNode* var = TNode::makeVarName(t);
        TNode* readS = TNode::makeReadStmt(var);
        TNode* stmtLst = TNode::makeStmtLst({ readS });
        Token* main = new Token(TokenType::name, "main", {0,0}, {0,0});
        TNode* procedure = TNode::makeProcedure(main, stmtLst);
        TNode* program = TNode::makeProgram(procedure);
        REQUIRE(*ast == *program);
    }


}

TEST_CASE("Parser: printRecursive") {

}

TEST_CASE("Parser: call") {

}

TEST_CASE("Parser: while") {

}

TEST_CASE("Parser: if") {

}

TEST_CASE("Parser: assignment") {

}

TEST_CASE("Parser: boolean expressions") {

}

TEST_CASE("Parser: relational expressions") {

}

TEST_CASE("Parser: arithmetic expressions") {

}




