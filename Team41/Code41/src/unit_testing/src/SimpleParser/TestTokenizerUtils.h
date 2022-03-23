#pragma once

#include <string>
#include <vector>
#include "SimpleParser/Token.h"

using namespace std;

class TestTokenizerUtils {
public:
    static void tokenize(string s);

    static void tokenizeAndCompare(string s, vector<pair<TokenType, string>> expected);
};
