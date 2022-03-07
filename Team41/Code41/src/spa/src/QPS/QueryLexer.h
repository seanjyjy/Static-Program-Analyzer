#pragma once

#include <string>
#include <unordered_set>
#include <optional>

using namespace  std;

class QueryLexer {
private:
    // keywords looked for during the declaration part of a query
    const unordered_set<string> declaration_keywords =
            {"stmt", "read", "print", "call", "while",
             "if", "assign", "variable", "constant", "procedure"};

    // keywords looked for during the clauses part of a query
    const unordered_set<string> clause_keywords =
            {"Follows", "Follows*", "Parent",
             "Parent*", "Uses", "Modifies",
             "Calls", "Calls*", "Next", "Next*",
             "Affects", "Affects*"};

    const unordered_set<char> special_char =
            {'(', ')', ';', ',', ' ', '\n'};

    int index = 0; // tracks position in input
    const string &input; // input string being parsed

    /**
     * Moves the index to the nearest non-space character in input.
     *
     * @return the new index position of the input string.
     */
    int skipSpaces();

    /**
     * Checks if a given character is within the special char vector.
     *
     * @param w the character to be checked.
     * @return boolean indicating if parameter is indeed in the vector.
     */
    bool isSpecialChar(char w);

    /**
     * Checks if a given string is within the declaration keywords vector.
     *
     * @param w the string to be checked.
     * @return boolean indicating if parameter is indeed in the vector.
     */
    bool isDeclarationKeyword(string w);

    /**
     * Checks if a given string is within the clause keywords vector.
     *
     * @param w the string to be checked.
     * @return boolean indicating if parameter is indeed in the vector.
     */
    bool isClauseKeyword(string w);

public:
    /**
     * Constructor for the QueryLexer class.
     *
     * @param input string being parsed.
     */
    QueryLexer(const string &input);

    /**
     * Returns the next token parsed in the input.
     * index is updated too.
     *
     * @return string representing the found token.
     */
    string nextToken();

    /**
     * Returns a declaration type expected in the next token.
     * index is updated too.
     *
     * @return optional string representing either a found declaration type or null
     */
    optional<string> nextDeclarationType();

    /**
     * Returns a synonym expected in the next token.
     * index is updated too.
     *
     * @return optional string representing either a found synonym or null
     */
    optional<string> nextSynonym();

    /**
     * Returns a non-expected but special expected in the next token.
     * index is updated too.
     *
     * @param w the string expected to show up.
     * @return optional string representing either a found special character or null.
     */
    optional<string> nextExpected(string w);

    /**
     * Returns a clause in the next token.
     * index is updated too.
     *
     * @return optional string representing either a found clause or null.
     */
    optional<string> nextClause();

    /**
     * Returns a clause variable in the next token.
     * index is updated too.
     *
     * @return optional string representing either a found clause variable or null.
     */
    optional<string> nextClauseVariable();

    /**
     * Returns a pattern expression in the next token.
     * index is updated too.
     *
     * @return optional string representing either a found pattern expression or null.
     */
    optional<string> nextPatternExpression();

    /**
     * Peeks if the next token is a certain string.
     * index is NOT updated too.
     *
     * @param w string to compare with the next token.
     * @return boolean indicating if the next token is indeed the param.
     */
    bool peekNextIsString(string w);

    /**
     * Checks if lexer has reached end of query.
     *
     * @return boolean indicating if end of query reached.
     */
    bool isEndOfQuery();

    /**
     * Checks if a string qualifies as a stmtRef.
     * stmtRef : synonym | '_' | INTEGER
     *
     * @param w string to be checked.
     * @return boolean indicating if string qualifies as stmtRef.
     */
    bool isStmtRef(string w);

    /**
     * Checks if a string qualifies as a entRef.
     * synonym | '_' | '"' IDENT '"'
     *
     * @param w string to be checked.
     * @return boolean indicating if string qualifies as entRef.
     */
    bool isEntRef(string w);

    /**
     * Checks if a string qualifies as a synonym in PQL.
     * synonym : IDENT
     * IDENT : LETTER ( LETTER | DIGIT )*
     *
     * @param w string to be checked.
     * @return boolean indicating if string qualifies as synonym.
     */
    bool isValidSynonym(string w);

    /**
     * Checks if a string qualifies as an integer in PQL.
     * INTEGER : DIGIT+
     * DIGIT: 0-9
     *
     * @param w string to be checked.
     * @return boolean indicating if string qualifies as integer.
     */
    bool isInteger(string w);

    /**
     * Checks if a string qualifies as an identifier in PQL.
     * '"' IDENT '"'
     *
     * @param w string to be checked.
     * @return boolean indicating if string qualifies as identifier.
     */
    bool isIdentifier(string w);

    /**
     * Checks if a string qualifies as an identifier in PQL.
     * "_"
     *
     * @param w string to be checked.
     * @return boolean indicating if string qualifies as a wildcard symbol.
     */
    bool isWildCard(string w);
};
