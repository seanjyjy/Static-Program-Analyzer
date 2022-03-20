#include "Tokenizer.h"
#include "Tokens.h"
#include "SimpleParser/SPUtils.h"
#include "Exception/TokenizeException.h"

#include <utility>
#include <cctype>
#include <stdexcept>
#include <iostream>

// member initialization
Tokenizer::Tokenizer(string s) : input(s), idx(0), row(0), col(0) {
    if (s.empty()) throw TokenizeException("input to tokenizer must be nonempty");
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
    idx++;
    col++;
    if (currToken == '\n') {
        row++;
        col = 0;
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
    if (n > (input.size() - idx)) throw TokenizeException("eating too many characters: " + to_string(n));

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
    ensure('{');
    return eatN(TokenType::openingBrace, 1);
}

Token Tokenizer::eatClosingBrace() {
    ensure('}');
    return eatN(TokenType::closingBrace, 1);
}

void Tokenizer::eatWhitespace() {
    if (!isspace(currToken)) throw runtime_error("current token should be <space>");
    advance();
}

Token Tokenizer::eatOpeningBracket() {
    ensure('(');
    return eatN(TokenType::openingBracket, 1);
}

Token Tokenizer::eatClosingBracket() {
    ensure(')');
    return eatN(TokenType::closingBracket, 1);
}

Token Tokenizer::eatSemicolon() {
    ensure(';');
    return eatN(TokenType::semicolon, 1);
}

Token Tokenizer::eatPlus() {
    ensure('+');
    return eatN(TokenType::plus, 1);
}

Token Tokenizer::eatMinus() {
    ensure('-');
    return eatN(TokenType::minus, 1);
}

Token Tokenizer::eatTimes() {
    ensure('*');
    return eatN(TokenType::times, 1);
}

Token Tokenizer::eatDiv() {
    ensure('/');
    return eatN(TokenType::div, 1);
}

Token Tokenizer::eatMod() {
    ensure('%');
    return eatN(TokenType::mod, 1);
}

Token Tokenizer::eatAnd() {
    ensure('&');
    return eatN(TokenType::andOp, 2);
}

Token Tokenizer::eatOr() {
    ensure('|');
    return eatN(TokenType::orOp, 2);
}

Token Tokenizer::eatGe() {
    ensure('>');
    return eatN(TokenType::ge, 2);
}

Token Tokenizer::eatGt() {
    ensure('>');
    return eatN(TokenType::gt, 1);
}

Token Tokenizer::eatLe() {
    ensure('<');
    return eatN(TokenType::le, 2);
}

Token Tokenizer::eatLt() {
    ensure('<');
    return eatN(TokenType::lt, 1);
}

Token Tokenizer::eatEq() {
    ensure('=');
    return eatN(TokenType::eq, 2);
}

Token Tokenizer::eatAssign() {
    ensure('=');
    return eatN(TokenType::assign, 1);
}

Token Tokenizer::eatNe() {
    ensure('!');
    return eatN(TokenType::ne, 2);
}

Token Tokenizer::eatNot() {
    ensure('!');
    return eatN(TokenType::notOp, 1);
}

Token Tokenizer::eatName() {
    if (!isalpha(currToken)) throw runtime_error("current token should be alphabet");
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
    if (!isdigit(currToken)) throw runtime_error("current token should be digit");
    pair<int, int> start = {row, col};

    string val;
    while (isdigit(currToken)) {
        val += currToken;
        advance();
    }

    // 0 allowed, but 01 should not be allowed
    if (val.size() > 1 && val[0] == '0') {
        string msg = withPosition("integers must not have leading zeroes") +
                     highlightSource(start.first, start.second, row, col);
        throw TokenizeException(msg);
    }

    pair<int, int> end = {row, col};
    return Token{TokenType::integer, val, start, end};
}

// public functions
Tokens Tokenizer::tokenize() {
    try {
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
                string msg = withPosition("unknown token " + string(1, currToken)) +
                             highlightSource(row, col, row, col);
                throw TokenizeException(msg);
            }

            tokens.add(tok);
        }

        Token eof = Token{TokenType::eof, "", {-1, -1}, {-1, -1}};
        tokens.add(eof);
        return tokens;
    } catch (TokenizeException &e) {
        cout << e.what() << endl;
        throw;
    }
}

string Tokenizer::highlightSource(int fromRow, int fromCol, int toRow, int toCol) {
    return SPUtils::highlightAndBanner(
            input, fromRow, fromCol, toRow, toCol,
            "vvvvvvvvvvvvvvvvvvvvvvvvvvvvv TOKENIZER ERROR HIGHLIGHT vvvvvvvvvvvvvvvvvvvvvvvvvvvvv",
            "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ END ERROR HIGHLIGHT ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
    );
}

void Tokenizer::ensure(char c) {
    if (currToken != c) throw runtime_error("current token should be " + string(1, c));
}
