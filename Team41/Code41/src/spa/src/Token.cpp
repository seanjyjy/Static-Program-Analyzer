//
// Created by Kendrick on 26/1/2022.
//

#include "Token.h"

#include <utility>

Token::Token() = default;

Token::Token(TokenType type, string val, pair<int, int> start, pair<int, int> end): type(type), val(std::move(val)), start(std::move(start)), end(std::move(end)){}

TokenType Token::getType() const {
    return type;
}

const string &Token::getVal() const {
    return val;
}

const pair<int, int> &Token::getStart() const {
    return start;
}

const pair<int, int> &Token::getEnd() const {
    return end;
}
