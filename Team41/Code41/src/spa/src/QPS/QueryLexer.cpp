#include "QueryLexer.h"

#include <ctype.h> // for isalpha, isdigit

using namespace std;

QueryLexer::QueryLexer(const string &input) : input(input) {}

int QueryLexer::skipSpaces() {
    if (index >= input.length()) {
        return index;
    }
    char curr = input.at(index);
    while (curr == ' ' || curr == '\n') {
        index++;
        if (index < input.length()) {
            curr = input.at(index);
        } else { break; }
    }
    return index;
}

bool QueryLexer::isSpecialChar(char w) {
    return special_char.find(w) != special_char.end();
}

string QueryLexer::nextToken() {
    if (index >= input.length()) {
        return "";
    }
    string out = "";
    skipSpaces();
    char curr = input.at(index);

    // Treat special characters as tokens themselves.
    if (isSpecialChar(curr)) {
        out.push_back(curr);
        return out;
    }
    while (!isSpecialChar(curr)) {
        if (isSpecialChar(curr)) {
            break;
        } else {
            out.push_back(curr);
        }
        index++;
        if (index >= input.length()) {
            break;
        }
        curr = input.at(index);
    }
    return out;
}

bool QueryLexer::isValidSynonym(string w) {
    /*
     IDENT : LETTER ( LETTER | DIGIT )*
     synonym : IDENT
     */
    for (int i = 0; i < w.length(); i++) {
        char c = w.at(i);
        if (i == 0) { // first char must be LETTER
            if (!isalpha(c)) {
                return false;
            }
        } else {
            if (!isalpha(c) && !isdigit(c)) {
                return false;
            }
        }
    }
    return true;
}

bool QueryLexer::isDeclarationKeyword(string w) {
    return declaration_keywords.find(w) != declaration_keywords.end();
}

bool QueryLexer::isClauseKeyword(string w) {
    return clause_keywords.find(w) != clause_keywords.end();
}

bool QueryLexer::isInteger(string w) {
    string::const_iterator it = w.begin();
    while (it != w.end() && isdigit(*it)) ++it;
    return !w.empty() && it == w.end();
}

bool QueryLexer::isIdentifier(string w) {
    // Must begin and end with quotes
    if (w.at(0) != '"') return false;
    if (w.at(w.length() - 1) != '"') return false;
    string content = w.substr(1, w.length() - 2);
    return isValidSynonym(content);
}

bool QueryLexer::isWildCard(string w) {
    return w == "_";
}

optional<string> QueryLexer::nextDeclarationType() {
    int temp = index;
    string possibleType = nextToken();
    if (isDeclarationKeyword(possibleType)) {
        return possibleType;
    } else {
        index = temp; // reset index to stable loc
        return nullopt;
    }
}

optional<string> QueryLexer::nextSynonym() {
    int temp = index;
    string possibleSynonym = nextToken();
    if (isValidSynonym(possibleSynonym)) {
        return possibleSynonym;
    } else {
        index = temp; // reset index to stable loc
        return nullopt;
    }
}

optional<string> QueryLexer::nextExpected(string w) {
    string expectedSpecial = nextToken();
    if (expectedSpecial == w) {
        index++;
        return w;
    } else {
        return nullopt;
    }
}

optional<string> QueryLexer::nextClause() {
    int temp = index;
    string possibleClause = nextToken();
    if (isClauseKeyword(possibleClause)) {
        return possibleClause;
    } else {
        index = temp; // reset index to stable loc
        return nullopt;
    }
}

optional<string> QueryLexer::nextClauseVariable() {
    string possibleVariable = nextToken();
    if (possibleVariable.length() == 0) {
        return nullopt;
    }
    if (possibleVariable == "_")
        return possibleVariable; // wild card

    if (isInteger(possibleVariable))
        return possibleVariable; // integer

    if (isValidSynonym(possibleVariable))
        return possibleVariable;

    if (isIdentifier(possibleVariable)) {
        return possibleVariable;
    }

    return nullopt;
}

optional<string> QueryLexer::nextPatternExpression() {
    if (isEndOfQuery()) {
        // No expected pattern expression found
        return nullopt;
    }
    string out = "";
    if (peekNextIsString("_")) {
        index++; // advance past first '_'
        if (peekNextIsString(")")) {
            // Wildcard encountered
            return "_";
        }
        // parse in sub pattern
        if (peekNextIsString("\"")) {
            index++;
        } else {
            // '"' expected at beginning of sub patterns
            return nullopt;
        }
        // sub patterns end before a '"'
        while (!peekNextIsString("\"")) {
            if (isEndOfQuery()) {
                // Pattern incomplete
                return nullopt;
            }
            out += input.at(index);
            index++;
        }
        index++; // Skip over closing '"'
        if (!peekNextIsString("_")) {
            // '_' expected at end of sub pattern
            return nullopt;
        }
        index++; // Skip over last '_'
        return "_" + out + "_"; // For parser to recognize sub
    } else {
        if (peekNextIsString("\"")) {
            index++;
        } else {
            // '\' expected at beginning of full patterns
            return nullopt;
        }
        // full patterns end before a '"'
        while (!peekNextIsString("\"")) {
            if (isEndOfQuery()) {
                // Pattern incomplete
                return nullopt;
            }
            out += input.at(index);
            index++;
        }
        index++; // Skip over closing '"'
        return out;
    }
}

bool QueryLexer::isEndOfQuery() {
    skipSpaces();
    return index == input.length();
}

bool QueryLexer::peekNextIsString(string w) {
    skipSpaces();
    for (int i = 0; i < w.length(); i++) {
        int j = index + i;
        if (j >= input.length()) {
            return false;
        }

        if (input.at(j) != w.at(i))
            return false;
    }
    return true;
}

bool QueryLexer::isStmtRef(string w) {
    return isValidSynonym(w) || w == "_" || isInteger(w);
}

bool QueryLexer::isEntRef(string w) {
    return isValidSynonym(w) || w == "_" || isIdentifier(w);
}
