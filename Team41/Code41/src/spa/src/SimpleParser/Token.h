#pragma once

#include <string>

#include "TokenType.h"

using namespace std;

class Token {
private:
    TokenType type;
    string val;
    pair<int, int> start; // starting position of token, { row, col }
    pair<int, int> end; // ending position of token, { row, col }
public:
    Token();
    Token(TokenType type, string val, pair<int, int> start, pair<int, int> end);
    TokenType getType() const;
    const string getVal() const;
    const pair<int, int> getStart() const;
    const pair<int, int> getEnd() const;
    const string toString() const;
    static string typeToString(TokenType type);
};
