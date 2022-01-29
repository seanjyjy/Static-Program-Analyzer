#pragma once

#include <vector>

#include "Token.h"

class Tokens {
private:
    vector<Token> tokens;
public:
    Tokens();
    void add(Token &t);
    size_t size();
    bool empty();
    Token &operator[](int idx);
};
