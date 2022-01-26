//
// Created by Kendrick on 26/1/2022.
//

#ifndef SPA_TOKEN_H
#define SPA_TOKEN_H

#include <string>

using namespace std;

enum class TokenType {
    openingBrace,
    closingBrace,
    openingBracket,
    closingBracket,
    assign,
    semicolon,
    notOp,
    andOp,
    orOp,
    gt,
    ge,
    lt,
    le,
    eq,
    ne,
    plus,
    minus,
    times,
    div,
    mod,
    procedure,
    readOp,
    printOp,
    callOp,
    whileOp,
    ifOp,
    name,
    integer,
};

class Token {
private:
    TokenType type;
    string val;
    pair<int, int> start; // starting position of token, { row, col }
    pair<int, int> end; // ending position of token, { row, col }
public:
    Token();
    Token(TokenType type, string val, pair<int, int> start, pair<int, int> end);
    TokenType getType() const;
    const string &getVal() const;
    const pair<int, int> &getStart() const;
    const pair<int, int> &getEnd() const;
    const string toString() const;
    static string typeToString(TokenType type);
};

#endif //SPA_TOKEN_H
