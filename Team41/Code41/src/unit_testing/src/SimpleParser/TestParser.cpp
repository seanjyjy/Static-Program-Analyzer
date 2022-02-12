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
