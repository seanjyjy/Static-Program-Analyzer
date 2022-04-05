#pragma once

#include <string>

#include "TokenType.h"

using namespace std;

class Token {
private:
    TokenType type;
    string val; // value as consumed from source file
    pair<int, int> start; // starting position of token, { row, col }
    pair<int, int> end; // ending position of token, { row, col }

    // for testing purposes, initializes start/end pairs to rubbish values
    Token(TokenType type, string val);

public:
    // for ast builder/testing purposes, initializes start/end pairs to rubbish values
    static Token *makeName(string val);

    // for ast builder/testing purposes, initializes start/end pairs to rubbish values
    static Token *makeConst(string val);

    Token();

    Token(TokenType type, string val, pair<int, int> start, pair<int, int> end);

    // returns the type of this token
    [[nodiscard]] TokenType getType() const;

    // returns the value of this token
    [[nodiscard]] string getVal() const;

    // returns the start position in source file this token was tokenized
    [[nodiscard]] pair<int, int> getStart() const;

    // returns the end position in source file this token was tokenized
    [[nodiscard]] pair<int, int> getEnd() const;

    // returns a deep copy of the current token
    [[nodiscard]] Token *copy() const;

};
