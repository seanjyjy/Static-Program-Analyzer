#include <utility>
#include <iostream>
#include "SimpleParser/Tokenizer.h"
#include "TestTokenizerUtils.h"

#include "catch.hpp"

using namespace std;

void TestTokenizerUtils::tokenizeAndCompare(string s, vector<pair<TokenType, string>> expected) {
    Tokenizer tokenizer(move(s));
    Tokens tokens = tokenizer.tokenize();
    REQUIRE(tokens.size() == expected.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        auto [expectedType, expectedVal] = expected[i];
        Token t = tokens[i];
        TokenType gotType = t.getType();
        string gotVal = t.getVal();
        REQUIRE(expectedType == gotType);
        REQUIRE(expectedVal == gotVal);
    }
}

void TestTokenizerUtils::tokenize(string s) {
    Tokenizer tokenizer(move(s));
    Tokens tokens = tokenizer.tokenize();
}
