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
        curr = input.at(index);
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
    string possibleType = nextToken();
    if (isDeclarationKeyword(possibleType)) {
        return possibleType;
    } else {
        // todo: throw something?
        printf("Invalid declaration type: <%s>", possibleType.c_str());
        return nullopt;
    }
}

optional<string> QueryLexer::nextSynonym() {
    string possibleSynonym = nextToken();
    if (isValidSynonym(possibleSynonym)) {
        return possibleSynonym;
    } else {
        // todo: throw something?
        printf("Invalid synonym: <%s>", possibleSynonym.c_str());
        return nullopt;
    }
}

optional<string> QueryLexer::nextSpecialExpected(string w) {
    string expected = nextToken();
    if (expected == w) {
        index++;
        return w;
    } else {
        // todo: throw something?
        printf("Syntax error: Expected '%s'.", w.c_str());
        return nullopt;
    }
}

optional<string> QueryLexer::nextSpecial(string w) {
    string expectedSpecial = nextToken();
    if (expectedSpecial == w) {
        return w;
    } else {
        return nullopt;
    }
}

optional<string> QueryLexer::nextClause() {
    string possibleClause = nextToken();
    if (isClauseKeyword(possibleClause)) {
        return possibleClause;
    } else {
        // todo: throw something?
        printf("Invalid clause: <%s>", possibleClause.c_str());
        return nullopt;
    }
}

optional<string> QueryLexer::nextClauseVariable() {
    string possibleVariable = nextToken();
    if (possibleVariable.length() == 0) {
        printf("Cannot leave clause empty. Requires 2 parameters.");
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
        printf("Expected a pattern expression.\n");
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
            printf("'\"' expected at beginning of sub patterns.\n");
            return nullopt;
        }
        // sub patterns end before a '"'
        while (!peekNextIsString("\"")) {
            if (isEndOfQuery()) {
                printf("Pattern incomplete.\n");
                return nullopt;
            }
            out += input.at(index);
            index++;
        }
        index++; // Skip over closing '"'
        if (!peekNextIsString("_")) {
            printf("'_' expected at end of sub pattern.");
            return nullopt;
        }
        index++; // Skip over last '_'
        return "_" + out + "_"; // For parser to recognize sub
    } else {
        if (peekNextIsString("\"")) {
            index++;
        } else {
            printf("'\"' expected at beginning of full patterns.\n");
            return nullopt;
        }
        // full patterns end before a '"'
        while (!peekNextIsString("\"")) {
            if (isEndOfQuery()) {
                printf("Pattern incomplete.\n");
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
