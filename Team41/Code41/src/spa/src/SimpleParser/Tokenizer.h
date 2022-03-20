#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_set>

#include "Token.h"
#include "Tokens.h"

using namespace std;

class Tokenizer {
private:
    const char EOF_CHAR = '\0';

    string input; // input string to tokenize
    int idx; // current position in input
    int row; // current row in input
    int col; // current col in input
    char currToken; // current char in input

    /**
     * Moves the current index by one and updates tokenizer state.
     */
    void advance();

    /**
     * Updates current row and column depending on current token.
     */
    void advancePosition();

    /**
     * Returns the next n characters (including the current char), or less if insufficient chars remain.
     * @param n the number of chars to lookahead
     * @return the next n characters, or less
     */
    string peek(int n);

    /**
     * Returns true if all characters have been consumed, false otherwise.
     */
    bool isEof();

    /**
     * Checks that the current token matches the given char.
     */
    void ensure(char c);

    /**
     * Adds the current row and col as a prefix to the input string.
     *
     * @param s the string to augment.
     * @return the augmented string.
     */
    string withPosition(const string &s);

    /**
     * Highlights the SIMPLE source code.
     *
     * @return the highlighted SIMPLE source code.
     */
    string highlightSource(int fromRow, int fromCol, int toRow, int toCol);

    /**
     * Consumes the current char while isspace() returns true on the current token.
     */
    void eatWhitespace();

    /**
     * Consumes n characters (or less if insufficient chars remain) and returns a token with the given type.
     *
     * @param type the type of token to return
     * @param n the number of chars to consume
     * @return the token with given type
     */
    Token eatN(TokenType type, int n);

    /**
     * Consumes '{'.
     *
     * @return a token with type openingBracket and value '{'
     */
    Token eatOpeningBrace();

    /**
     * Consumes '}'.
     *
     * @return a token with type closingBrace and value '}'
     */
    Token eatClosingBrace();

    /**
     * Consumes '('.
     *
     * @return a token with type openingBracket and value '('
     */
    Token eatOpeningBracket();

    /**
     * Consumes ')'.
     *
     * @return a token with type closingBracket and value ')'
     */
    Token eatClosingBracket();

    /**
     * Consumes ';'.
     *
     * @return a token with type semicolon and value ';'
     */
    Token eatSemicolon();

    /**
     * Consumes '+'.
     *
     * @return a token with type plus and value '+'
     */
    Token eatPlus();

    /**
     * Consumes '-'.
     *
     * @return a token with type minus and value '-'
     */
    Token eatMinus();

    /**
     * Consumes '*'.
     *
     * @return a token with type times and value '*'
     */
    Token eatTimes();

    /**
     * Consumes '/'.
     *
     * @return a token with type div and value '/'
     */
    Token eatDiv();

    /**
     * Consumes '%'.
     *
     * @return a token with type mod and value '%'
     */
    Token eatMod();

    /**
     * Consumes '&&'.
     *
     * @return a token with type andOp and value '&&'
     */
    Token eatAnd();

    /**
     * Consumes '||'.
     *
     * @return a token with type orOp and value '||'
     */
    Token eatOr();

    /**
     * Consumes '>='.
     *
     * @return a token with type ge and value '>='
     */
    Token eatGe();

    /**
     * Consumes '>'.
     *
     * @return a token with type gt and value '>'
     */
    Token eatGt();

    /**
     * Consumes '<='.
     *
     * @return a token with type le and value '<='
     */
    Token eatLe();

    /**
     * Consumes '<'.
     *
     * @return a token with type lt and value '<'
     */
    Token eatLt();

    /**
     * Consumes '=='.
     *
     * @return a token with type eq and value '=='
     */
    Token eatEq();

    /**
     * Consumes '='.
     *
     * @return a token with type assign and value '='
     */
    Token eatAssign();

    /**
     * Consumes '!='.
     *
     * @return a token with type ne and value '!='
     */
    Token eatNe();

    /**
     * Consumes '!'.
     *
     * @return a token with type notOp and value '!'
     */
    Token eatNot();

    /**
     * Consumes a name.
     *
     * @return a token with type name and what was consumed as value.
     */
    Token eatName();

    /**
     * Consumes an integer.
     *
     * @return a token with type integer and what was consumed as value.
     */
    Token eatInteger();

public:
    Tokenizer(string s);

    /**
     * Converts the given input into tokens.
     *
     * @return the Token class wrapping a list of tokens.
     */
    Tokens tokenize();
};
