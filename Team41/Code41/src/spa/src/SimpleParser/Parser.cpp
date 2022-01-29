#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

#include "PKB.h"
#include "Common/TNode.h"
#include "Parser.h"
#include "Tokenizer.h"

Parser::Parser(): cursor(0) {
    if (tokens.empty()) throw runtime_error("parser tokens must not be empty");
    currToken = tokens[0];
}

void Parser::advance() {
    cursor++;
    assert(cursor < tokens.size()); // last token is eof
    currToken = tokens[cursor];
}

int Parser::saveCursor() {
    return cursor;
}

void Parser::backtrack(int to) {
    assert(to >= 0 && to < tokens.size());
    cursor = to;
    currToken = tokens[cursor];
}

TNode &Parser::eatProgram() {

}

TNode Parser::parse(string &input) {
    Tokenizer tokenizer(input);
    tokens = tokenizer.tokenize();
    return eatProgram();
}

