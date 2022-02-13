#pragma once

#include <string>
#include <unordered_set>
#include <optional>

using namespace  std;

class QueryLexer {
private:
    const unordered_set<string> declaration_keywords =
            {"stmt", "read", "print", "call", "while",
             "if", "assign", "variable", "constant", "procedure"};
    const unordered_set<string> clause_keywords =
            {"Follows", "Follows*", "Parent",
             "Parent*", "Uses", "Modifies"}; // Pattern done separately
    const unordered_set<char> special_char =
            {'(', ')', ';', ',', ' '};

    int index = 0;
    const string &input;
    string currToken = "";

    int skipSpaces();
    bool isSpecialChar(char w);
    bool isDeclarationKeyword(string w);
    bool isClauseKeyword(string w);

public:
    QueryLexer(const string &input);

    string nextToken();
    optional<string> nextDeclarationType();
    optional<string> nextSynonym();
    optional<string> nextSpecialExpected(string w);
    optional<string> nextSpecial(string w);
    optional<string> nextClause();
    optional<string> nextClauseVariable();
    optional<string> nextPatternExpression();
    bool peekNextIsString(string w);
    bool isEndOfQuery();
    bool isStmtRef(string w);
    bool isEntRef(string w);
    bool isValidSynonym(string w);
    bool isInteger(string w);
    bool isIdentifier(string w);
    bool isWildCard(string w);
};
