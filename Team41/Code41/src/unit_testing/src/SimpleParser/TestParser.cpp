#include <iostream>

#include "SimpleParser/Parser.h"
#include "Common/TreeUtils.h"
#include "catch.hpp"

using namespace std;

// TODO: actually generate the ast and check equality
TEST_CASE("Parser: procedure") {
    SECTION("single procedure should parse") {
        string s = "procedure main {"
                   "print x;"
                   "}";
        Parser p;
        TNode* ast = p.parseProgram(s);
        REQUIRE(ast != nullptr);
        ast->printRecursive();
    }

    SECTION("multiple procedures should parse") {
        string s = "procedure one {\n"
                   "\tprint x;\n"
                   "}\n"
                   "\n"
                   "procedure two {\n"
                   "\tprint y;\n"
                   "}";
        Parser p;
        TNode* ast = p.parseProgram(s);
        REQUIRE(ast != nullptr);
        ast->printRecursive();
    }

    SECTION("keyword as procedure name should parse") {
        string s = "procedure procedure { print x; }\n"
                   "procedure read { print x; }\n"
                   "procedure print { print x; }\n"
                   "procedure call { print x; }\n"
                   "procedure while { print x; }\n"
                   "procedure if { print x; }\n"
                   "procedure assign { print x; }";
        Parser p;
        TNode* ast = p.parseProgram(s);
        REQUIRE(ast != nullptr);
        ast->printRecursive();
    }
}

TEST_CASE("Parser: read") {
    SECTION("1") {
        string s = "procedure main {\n"
                   "\tread x;\n"
                   "}";
        Parser p;
        TNode* ast = p.parseProgram(s);
        REQUIRE(ast != nullptr);
        ast->printRecursive();

        Token* t = new Token(TokenType::name, "x", {0,0}, {0,0});
        TNode* var = TNode::makeVarName(t);
        TNode* readS = TNode::makeReadStmt(var);
        TNode* stmtLst = TNode::makeStmtLst({ readS });
        Token* main = new Token(TokenType::name, "main", {0,0}, {0,0});
        TNode* procedure = TNode::makeProcedure(main, stmtLst);
        TNode* program = TNode::makeProgram({ procedure });
        program->printRecursive();
        REQUIRE(TreeUtils::isEqual(program, ast));
    }
}

TEST_CASE("Parser: print") {
    SECTION("1") {
        string s = "procedure main {"
                   "print x;"
                   "}";

        Parser p;
        TNode* ast = p.parseProgram(s);
        REQUIRE(ast != nullptr);
        ast->printRecursive();
    }
}

TEST_CASE("Parser: call") {
    string s = "procedure main {"
               "call helloWorld;"
               "}";

    Parser p;
    TNode* ast = p.parseProgram(s);
    REQUIRE(ast != nullptr);
    ast->printRecursive();
}

TEST_CASE("Parser: while") {
    string s = "procedure main {"
               "while (2 > 1) {"
               "print x;"
               "}"
               "}";

    Parser p;
    TNode* ast = p.parseProgram(s);
    REQUIRE(ast != nullptr);
    ast->printRecursive();
}

TEST_CASE("Parser: if") {
    string s = "procedure main {"
               "if (2 > 1) then {"
               "print x;"
               "} else {"
               "print y;"
               "}"
               "}";
    Parser p;
    TNode* ast = p.parseProgram(s);
    REQUIRE(ast != nullptr);
    ast->printRecursive();
}

TEST_CASE("Parser: assignment") {
    string s = "procedure main {"
               "x = y;"
               "}";

    Parser p;
    TNode* ast = p.parseProgram(s);
    REQUIRE(ast != nullptr);
    ast->printRecursive();
}

TEST_CASE("Parser: boolean expressions") {
    string s = "procedure main {"
               "while (((1 == 1) || (1 != 1)) && (!(1 <= 1))) {"
               "print x;"
               "}"
               "}";

    Parser p;
    TNode* ast = p.parseProgram(s);
    REQUIRE(ast != nullptr);
    ast->printRecursive();
}

TEST_CASE("Parser: relational expressions") {
    string s = "procedure main {"
               "while ((((((x > 1) && (y >= 2)) && (z < 3)) && (a <= 4)) && (b == 5)) && (c != 6)) {"
               "print x;"
               "}"
               "}";

    Parser p;
    TNode* ast = p.parseProgram(s);
    REQUIRE(ast != nullptr);
    ast->printRecursive();
}

TEST_CASE("Parser: arithmetic expressions") {
    string s = "procedure main {"
               "x = 1 * 2 + 3 / 4;"
               "}";

    Parser p;
    TNode* ast = p.parseProgram(s);
    REQUIRE(ast != nullptr);
    ast->printRecursive();
}




