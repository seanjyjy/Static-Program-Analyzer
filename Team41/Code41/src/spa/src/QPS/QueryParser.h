#pragma once

#include <unordered_set>

#include "QueryObject.h"
#include "QueryDeclaration.h"
#include "QueryEvaluator.h"
#include "QueryLexer.h"
#include "SelectTarget.h"
#include "SimpleParser/Parser.h" // for pattern parse

using namespace std;

class QueryParser {
private:
    const string &input; // input string being parsed
    QueryObject *queryObject; // query data for evaluation
    QueryLexer *lex; // pointer to a lexer instance

    /**
     * Moves past the phrase "such that".
     *
     * @return boolean indicating if operation successful.
     */
    bool skipSuchThat();

    bool skipPattern();

    bool skipWith();

    /**
     * Checks if the combo of strings
     * constitute a valid PQL clause.
     *
     * @param type string indicating the supposed clause type.
     * @param left string representing the clause's LHS.
     * @param right string representing the clause's RHS.
     * @return boolean indicating if clause is valid.
     */
    bool isQueryClauseValid(string type, string left, string right);

    /**
     * Get the system representation of a clause's type.
     *
     * @param type string indicating the  clause type.
     * @param left string representing the clause's LHS.
     * @param right string representing the clause's RHS.
     * @return QueryClause::clause_type indicating the system clause type.
     */
    QueryClause::clause_type determineClauseType(string type, string left, string right);

    /**
     * Get the system representation of a variable's type.
     *
     * @param w string to be checked.
     * @return ClauseVariable::variable_typeuery indicating the system variable type.
     */
    ClauseVariable::variable_type determineVariableType(string w);

    /**
     * Parses the declarative portion of a query.
     * Populates the QueryObject with the new declaration(s).
     *
     * @return boolean if parsing was successful.
     */
    bool parseDeclarations();

    bool parseDeclarationsOfTypeString(string type);

    bool lookForDeclarationComma();

    bool lookForDeclarationSemiColon();

    void generateDeclarationObject(string type, string synonym);

    /**
     * Parses the synonym that follows a "Select" command.
     * Populates the QueryObject with the select synonym.
     *
     * @return boolean if parsing was successful.
     */
    bool parseSelectTarget();

    bool parseSelectTuple();

    bool parseSelectSingle();

    Selectable::attributeName parseSelectAttribute();

    bool parseSuchThatClauses();

    bool parsePatternClauses();

    bool parseWithClauses();

    bool parseWithClause();

    optional<WithVariable> parseWithRef();

    bool isValidAttrName(string w);

    WithVariable::attributeName toAttrName(string w);

    /**
     * Parses a clause of a query
     *
     * @return boolean if parsing was successful.
     */
    bool parseClause();

    optional<string> parseClauseType();

    optional<string> parseClauseVariable(string clause);

    bool lookForClauseGrammarSymbol(string symbol, string notFoundMessage);

    /**
     * Parses a "pattern" clause of a query
     * Populates the QueryObject with a new pattern clause instance.
     *
     * @return boolean if parsing was successful.
     */
    bool parsePatternClause();

    optional<QueryDeclaration> parsePatternSyn();

    optional<string> parsePatternLHS();

    optional<PatternVariable> parsePatternRHS();

    optional<vector<PatternVariable>> parseAssignPatternParams();

    optional<vector<PatternVariable>> parseWhilePatternParams();

    optional<vector<PatternVariable>> parseIfPatternParams();

    optional<vector<PatternVariable>> parseDummyPatternParams();

    void buildPatternClauseObject(QueryDeclaration patternSyn, string lhs, vector<PatternVariable> rhs);

    /**
     * Populates the QueryObject with a new clause instance.
     *
     * @param type string indicating the  clause type.
     * @param left string representing the clause's LHS.
     * @param right string representing the clause's RHS.
     * @return boolean if operation was successful.
     */
    bool buildClause(string clause, string left, string right);

    /**
     * Checks if a given synonym has been validly declared
     * during the declaration part of the query.
     *
     * @param synonym string to verify.
     * @return boolean if synonym was indeed declared.
     */
    bool isDeclared(string synonym);

    /**
     * Checks if a given procedure synonym had been declared
     * during the declaration part of the query.
     *
     * @param synonym string to verify.
     * @return boolean if procedure synonym was indeed declared.
     */
    bool isDeclaredProcedure(string synonym);

    /**
     * Gets the system type of a declared synonym
     *
     * @param synonym string to verify.
     * @return QueryDeclaration::design_entity_type of the synonym (assuming validly declared).
     */
    Entities* determineDeclarationType(string synonym);

    /**
     * Gets the system representation of a declared synonym.
     *
     * @param synonym string to lookup.
     * @return QueryDeclaration object of the synonym if declared, else null.
     */
    optional<QueryDeclaration> findMatchingDeclaration(string synonym);

    /**
     * Clean up method to clean up unused attributes after parsing
     */
    void cleanup();

public:
    /**
     * Constructor for the QueryParser class.
     *
     * @param input string being parsed.
     */
    QueryParser(string &input);

    /**
     * Parses the PQL input.
     *
     * @return QueryObject instance it has populated during the parsing.
     */
    QueryObject *parse();
};
