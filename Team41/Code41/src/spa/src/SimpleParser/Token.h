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

    // for testing purposes
    Token(TokenType type, string val);
public:
    // for testing purposes
    static Token* makeVar(string val);
    static Token* makeConst(string val);

    Token();

    Token(TokenType type, string val, pair<int, int> start, pair<int, int> end);

    TokenType getType() const;

    const string getVal() const;

    const pair<int, int> getStart() const;

    const pair<int, int> getEnd() const;

    const string toString() const;

    Token *copy() const;

    static string typeToString(TokenType type);
};
