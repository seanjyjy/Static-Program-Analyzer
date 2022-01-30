#include <iostream>

#include "Common/TNode.h"
#include "SimpleParser/Tokenizer.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("Tokenizer: all") {
    string s = "procedure main {\n"
               "    one = 0;\n"
               "    two = 0;\n"
               "    read x;\n"
               "    print x;\n"
               "    while (!(1 > 2) && (3 >= 4) {\n"
               "        one = 1;\n"
               "    }\n"
               "    if ((one != two) || (1 <= 2)) then {\n"
               "        two = 2;\n"
               "    } else {\n"
               "        two = 3;\n"
               "    }\n"
               "}";
    Tokenizer tokenizer(s);
    Tokens tokens = tokenizer.tokenize();
    vector<TokenType> expected = {
            TokenType::keyword,
            TokenType::name,
            TokenType::openingBrace,
            TokenType::name,
            TokenType::assign,
            TokenType::integer,
            TokenType::semicolon,
            TokenType::name,
            TokenType::assign,
            TokenType::integer,
            TokenType::semicolon,
            TokenType::keyword,
            TokenType::name,
            TokenType::semicolon,
            TokenType::keyword,
            TokenType::name,
            TokenType::semicolon,
            TokenType::keyword,
            TokenType::openingBracket,
            TokenType::notOp,
            TokenType::openingBracket,
            TokenType::integer,
            TokenType::gt,
            TokenType::integer,
            TokenType::closingBracket,
            TokenType::andOp,
            TokenType::openingBracket,
            TokenType::integer,
            TokenType::ge,
            TokenType::integer,
            TokenType::closingBracket,
            TokenType::openingBrace,
            TokenType::name,
            TokenType::assign,
            TokenType::integer,
            TokenType::semicolon,
            TokenType::closingBrace,
            TokenType::keyword,
            TokenType::openingBracket,
            TokenType::openingBracket,
            TokenType::name,
            TokenType::ne,
            TokenType::name,
            TokenType::closingBracket,
            TokenType::orOp,
            TokenType::openingBracket,
            TokenType::integer,
            TokenType::le,
            TokenType::integer,
            TokenType::closingBracket,
            TokenType::closingBracket,
            TokenType::keyword,
            TokenType::openingBrace,
            TokenType::name,
            TokenType::assign,
            TokenType::integer,
            TokenType::semicolon,
            TokenType::closingBrace,
            TokenType::keyword,
            TokenType::openingBrace,
            TokenType::name,
            TokenType::assign,
            TokenType::integer,
            TokenType::semicolon,
            TokenType::closingBrace,
            TokenType::closingBrace,
            TokenType::eof,
    };
    for (int i = 0; i < expected.size(); i++) {
        cout << i << endl;
        REQUIRE(expected[i] == tokens[i].getType());
    }
}
