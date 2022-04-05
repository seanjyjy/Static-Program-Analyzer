#include "Token.h"

#include <utility>
#include <stdexcept>

Token::Token() = default;

Token::Token(TokenType type, string val, pair<int, int> start, pair<int, int> end) : type(type), val(std::move(val)),
                                                                                     start(std::move(start)),
                                                                                     end(std::move(end)) {}

Token::Token(TokenType type, string val) : type(type), val(move(val)), start({-1, -1}), end({-1, -1}) {}

Token *Token::makeName(string val) {
    return new Token(TokenType::name, move(val));
}

Token *Token::makeConst(string val) {
    return new Token(TokenType::integer, move(val));
}

TokenType Token::getType() const {
    return type;
}

string Token::getVal() const {
    return val;
}

pair<int, int> Token::getStart() const {
    return start;
}

pair<int, int> Token::getEnd() const {
    return end;
}

Token *Token::copy() const {
    return new Token(getType(), getVal(), getStart(), getEnd());
}

