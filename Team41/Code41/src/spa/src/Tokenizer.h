//
// Created by Kendrick on 26/1/2022.
//

#ifndef SPA_TOKENIZER_H
#define SPA_TOKENIZER_H

#include <string>
#include <vector>
#include <functional>

#include "Token.h"

using namespace std;

class Tokenizer {
private:
    string input;
    int idx;
    int row;
    int col;
    char currToken;

    void advance();
    void advancePosition();
    string peek(int n);
    bool eof();
    void eatWhitespace();
    Token eatN(TokenType type, int n);
    Token eatOpeningBrace();
    Token eatClosingBrace();
    Token eatOpeningBracket();
    Token eatClosingBracket();
    Token eatSemicolon();
    Token eatPlus();
    Token eatMinus();
    Token eatTimes();
    Token eatDiv();
    Token eatMod();
    Token eatAnd();
    Token eatOr();
    Token eatGe();
    Token eatGt();
    Token eatLe();
    Token eatLt();
    Token eatEq();
    Token eatAssign();
    Token eatNe();
    Token eatNot();
    Token eatName();
    Token eatInteger();
public:
    Tokenizer(string s);
    vector<Token> tokenize();
};


#endif //SPA_TOKENIZER_H
