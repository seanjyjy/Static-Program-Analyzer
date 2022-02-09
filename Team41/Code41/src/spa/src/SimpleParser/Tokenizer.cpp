#include "Tokenizer.h"
#include "Tokens.h"

#include <utility>
#include <cctype>
#include <stdexcept>
#include <cassert>
#include <unordered_set>

// member initialization
Tokenizer::Tokenizer(string s) : input(s), idx(0), row(0), col(0) {
    if (s.empty()) throw invalid_argument("input to tokenizer must be nonempty");
    currToken = s[0];
}

// private functions
void Tokenizer::advance() {
    if (idx >= input.size()) {
        currToken = EOF_CHAR;
        return;
    }
    advancePosition();
    if (idx >= input.size()) {
        currToken = EOF_CHAR;
    } else {
        currToken = input[idx];
    }
}

void Tokenizer::advancePosition() {
    this->idx++;
    this->col++;
    if (currToken == '\n') {
        this->row++;
        this->col = 0;
    }
}

// returns n consecutive characters from the current index, or less than n if insufficient chars remain.
string Tokenizer::peek(int n) {
    return input.substr(idx, n);
}

bool Tokenizer::isEof() {
    return currToken == EOF_CHAR;
}

string Tokenizer::withPosition(const string &s) {
    string prefix = "row " + to_string(row) + ", col " + to_string(col) + ": ";
    return prefix + s;
}

Token Tokenizer::eatN(TokenType type, int n) {
    if (n > (input.size() - idx)) throw length_error("eating too many characters: " + to_string(n));

    pair<int, int> start = {row, col};
    string val;
    for (int i = 0; !isEof() && i < n; i++) {
        val += currToken;
        advance();
    }
    pair<int, int> end = {row, col};
    return Token{type, val, start, end};
}

Token Tokenizer::eatOpeningBrace() {
    assert(currToken == '{');
    return eatN(TokenType::openingBrace, 1);
}

Token Tokenizer::eatClosingBrace() {
    assert(currToken == '}');
    return eatN(TokenType::closingBrace, 1);
}

void Tokenizer::eatWhitespace() {
    assert(isspace(currToken));
    advance();
}

Token Tokenizer::eatOpeningBracket() {
    assert(currToken == '(');
    return eatN(TokenType::openingBracket, 1);
}

Token Tokenizer::eatClosingBracket() {
    assert(currToken == ')');
    return eatN(TokenType::closingBracket, 1);
}

Token Tokenizer::eatSemicolon() {
    assert(currToken == ';');
    return eatN(TokenType::semicolon, 1);
}

Token Tokenizer::eatPlus() {
    assert(currToken == '+');
    return eatN(TokenType::plus, 1);
}

Token Tokenizer::eatMinus() {
    assert(currToken == '-');
    return eatN(TokenType::minus, 1);
}

Token Tokenizer::eatTimes() {
    assert(currToken == '*');
    return eatN(TokenType::times, 1);
}

Token Tokenizer::eatDiv() {
    assert(currToken == '/');
    return eatN(TokenType::div, 1);
}

Token Tokenizer::eatMod() {
    assert(currToken == '%');
    return eatN(TokenType::mod, 1);
}

Token Tokenizer::eatAnd() {
    assert(currToken == '&');
    return eatN(TokenType::andOp, 2);
}

Token Tokenizer::eatOr() {
    assert(currToken == '|');
    return eatN(TokenType::orOp, 2);
}

Token Tokenizer::eatGe() {
    assert(currToken == '>');
    return eatN(TokenType::ge, 2);
}

Token Tokenizer::eatGt() {
    assert(currToken == '>');
    return eatN(TokenType::gt, 1);
}

Token Tokenizer::eatLe() {
    assert(currToken == '<');
    return eatN(TokenType::le, 2);
}

Token Tokenizer::eatLt() {
    assert(currToken == '<');
    return eatN(TokenType::lt, 1);
}

Token Tokenizer::eatEq() {
    assert(currToken == '=');
    return eatN(TokenType::eq, 2);
}

Token Tokenizer::eatAssign() {
    assert(currToken == '=');
    return eatN(TokenType::assign, 1);
}

Token Tokenizer::eatNe() {
    assert(currToken == '!');
    return eatN(TokenType::ne, 2);
}

Token Tokenizer::eatNot() {
    assert(currToken == '!');
    return eatN(TokenType::notOp, 1);
}

Token Tokenizer::eatName() {
    assert(isalpha(currToken));
    pair<int, int> start = {row, col};

    string val;
    do {
        val += currToken; // first token is
        advance();
    } while (!isEof() && isalnum(currToken));

    pair<int, int> end = {row, col};
    return Token{TokenType::name, val, start, end};
}

Token Tokenizer::eatInteger() {
    assert(isdigit(currToken));
    pair<int, int> start = {row, col};

    string val;
    while (isdigit(currToken)) {
        val += currToken;
        advance();
    }

    // 0 allowed, but 01 should not be allowed
    if (val.size() > 1 && val[0] == '0') throw runtime_error(withPosition("integers must not have leading zeroes"));

    pair<int, int> end = {row, col};
    return Token{TokenType::integer, val, start, end};
}

// public functions
Tokens Tokenizer::tokenize() {
    Tokens tokens;

    while (currToken != EOF_CHAR) {
        Token tok;

        if (isspace(currToken)) {
            eatWhitespace(); // no token
            continue;
        }

        if (currToken == '{') {
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
        } else if (isdigit(currToken)) {
            tok = eatInteger();
        } else {
            throw runtime_error(withPosition("unknown token " + string(1, currToken)));
        }

        tokens.add(tok);
    }

    Token eof = Token{TokenType::eof, "", {-1, -1}, {-1, -1}};
    tokens.add(eof);
    return tokens;
}
