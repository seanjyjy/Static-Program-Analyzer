#include "Tokens.h"

#include <vector>

#include "Token.h"

#include <stdexcept>

using namespace std;

Tokens::Tokens() = default;

void Tokens::add(Token &t) {
    tokens.push_back(t);
}

bool Tokens::empty() {
    return tokens.empty();
}

Token &Tokens::operator[](int idx) {
    if (idx < 0 || idx >= tokens.size()) throw out_of_range("vector out of bounds");
    return tokens[idx];
}

size_t Tokens::size() {
    return tokens.size();
}


