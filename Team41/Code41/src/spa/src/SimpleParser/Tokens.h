#pragma once

#include <vector>

#include "Token.h"

// Wrapper class around a list of tokens, mainly to provide easy printing
class Tokens {
private:
    vector<Token> tokens;
public:
    Tokens();

    // adds a token to the tokens list.
    void add(Token &t);

    // returns the size of the tokens list.
    size_t size();

    // returns true if the tokens list is empty, false otherwise.
    bool empty();

    // empties the tokens list.
    void clear();

    // same functionality as operator[] for vectors
    Token &operator[](int idx);

    // prints all tokens in the token list, for debugging purposes
    void print();
};
