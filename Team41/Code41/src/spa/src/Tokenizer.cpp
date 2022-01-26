//
// Created by Kendrick on 26/1/2022.
//

#include "Tokenizer.h"

#include <utility>
#include <cctype>
#include <stdexcept>
#include <cassert>

// member initialization
Tokenizer::Tokenizer(string s): input(std::move(s)), idx(0), row(0), col(0) {
    if (s.empty()) throw invalid_argument("input to tokenizer must be nonempty");
    currToken = s[0];
}

// private functions
void Tokenizer::advance() {
    if (idx >= input.size()) {
        currToken = '\0';
        return;
    }
    advancePosition();
}

void Tokenizer::advancePosition() {
    this->idx++;
    this->col++;
    if (currToken == '\n') {
        this->row++;
        this->col = 0;
    }
}

string Tokenizer::peek(int n) {
    return input.substr(idx, n);
}

bool Tokenizer::eof() {
    return currToken == '\0';
}

Token Tokenizer::eatN(TokenType type, int n) {
    // TODO: can we upgrade to C++20 so I can use #include <format>
    if (n > (input.size() - idx)) throw length_error("eating too many characters: " + to_string(n));

    pair<int, int> start = { row, col };
    string val = string(1, currToken);
    n--;
    for (int i = 0; i < n; i++) {
        advance();
        if (!eof()) val += currToken;
    }
    pair<int, int> end = { row, col };
    return Token{type, val, start, end};
}

Token Tokenizer::eatOpeningBrace() { return eatN(TokenType::openingBrace, 1); }
Token Tokenizer::eatClosingBrace() { return eatN(TokenType::closingBrace, 1); }
void Tokenizer::eatWhitespace() { advance(); }
Token Tokenizer::eatOpeningBracket() { return eatN(TokenType::openingBracket, 1); }
Token Tokenizer::eatClosingBracket() { return eatN(TokenType::closingBracket, 1); }
Token Tokenizer::eatSemicolon() { return eatN(TokenType::semicolon, 1); }
Token Tokenizer::eatPlus() { return eatN(TokenType::plus, 1); }
Token Tokenizer::eatMinus() { return eatN(TokenType::minus, 1); }
Token Tokenizer::eatTimes() { return eatN(TokenType::times, 1); }
Token Tokenizer::eatDiv() { return eatN(TokenType::div, 1); }
Token Tokenizer::eatMod() { return eatN(TokenType::mod, 1); }
Token Tokenizer::eatAnd() { return eatN(TokenType::andOp, 2); }
Token Tokenizer::eatOr() { return eatN(TokenType::orOp, 2); }
Token Tokenizer::eatGe() { return eatN(TokenType::ge, 2); }
Token Tokenizer::eatGt() { return eatN(TokenType::gt, 1); }
Token Tokenizer::eatLe() { return eatN(TokenType::le, 2); }
Token Tokenizer::eatLt() { return eatN(TokenType::lt, 1); }
Token Tokenizer::eatEq() { return eatN(TokenType::eq, 2); }
Token Tokenizer::eatAssign() { return eatN(TokenType::assign, 1); }
Token Tokenizer::eatNe() { return eatN(TokenType::ne, 2); }
Token Tokenizer::eatNot() { return eatN(TokenType::notOp, 1); }
Token Tokenizer::eatName() {
    assert(isalpha(currToken));
    pair<int, int> start = { row, col };

    string val;
    do {
        val += currToken; // first token is
        advance();
    } while (!eof() && isalnum(currToken));

    pair<int, int> end = { row, col };
    return Token{TokenType::name, val, start, end};
}
Token Tokenizer::eatInteger() {
    pair<int, int> start = { row, col };

    string val;
    while (isdigit(currToken)) {
        val += currToken;
        advance();
    }

    pair<int, int> end = { row, col };
    return Token{TokenType::integer, val, start, end};
}


// public functions
vector<Token> Tokenizer::tokenize() {
    vector<Token> tokens;

    size_t inputLen = input.size();
    while (currToken != '\0') {
        Token tok;

        if (isspace(currToken)) {
            eatWhitespace(); // no token
        } else if (currToken == '{') {
            tok = eatOpeningBrace();
        } else if (currToken == '}') {
            tok = eatClosingBrace();
        } else if (currToken == '(') {
            tok = eatOpeningBracket();
        } else if (currToken == ')') {
            tok = eatClosingBracket();
        } else if (currToken == ';') {
            tok = eatSemicolon();
        } else if (currToken == '+') {
            tok = eatPlus();
        } else if (currToken == '-') {
            tok = eatMinus();
        } else if (currToken == '*') {
            tok = eatTimes();
        } else if (currToken == '/') {
            tok = eatDiv();
        } else if (currToken == '%') {
            tok = eatMod();
        } else if (currToken == '&') {
            tok = eatAnd();
        } else if (currToken == '|') {
            tok = eatOr();
        } else if (currToken == '>' && peek(2) == ">=") {
            tok = eatGe();
        } else if (currToken == '>') {
            tok = eatGt();
        } else if (currToken == '<' && peek(2) == "<=") {
            tok = eatLe();
        } else if (currToken == '<') {
            tok = eatLt();
        } else if (currToken == '=' && peek(2) == "==") {
            tok = eatEq();
        } else if (currToken == '=') {
            tok = eatAssign();
        } else if (currToken == '!' && peek(2) == "!=") {
            tok = eatNe();
        } else if (currToken == '!') {
            tok = eatNot();
        } else if (isalpha(currToken)) {
            tok = eatName();
            if (tok.getVal() == "read") tok = Token{TokenType::readOp, tok.getVal(), tok.getStart(), tok.getEnd()};
            if (tok.getVal() == "print") tok = Token{TokenType::printOp, tok.getVal(), tok.getStart(), tok.getEnd()};
            if (tok.getVal() == "call") tok = Token{TokenType::callOp, tok.getVal(), tok.getStart(), tok.getEnd()};
            if (tok.getVal() == "while") tok = Token{TokenType::whileOp, tok.getVal(), tok.getStart(), tok.getEnd()};
            if (tok.getVal() == "if") tok = Token{TokenType::ifOp, tok.getVal(), tok.getStart(), tok.getEnd()};
        } else if (isdigit(currToken)) {
            tok = eatInteger();
        }

        tokens.push_back(tok);
    }

    return tokens;
}



