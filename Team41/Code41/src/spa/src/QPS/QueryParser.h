#pragma once

#include <unordered_set>

#include "QPS/QO/QueryObject.h"
#include "QPS/QO/QueryDeclaration.h"
#include "QueryEvaluator.h"
#include "QueryLexer.h"
#include "QPS/QO/SelectTarget.h"
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

    /**
     * Moves past the phrase "pattern".
     *
     * @return boolean indicating if operation successful.
     */
    bool skipPattern();

    /**
     * Moves past the phrase "with".
     *
     * @return boolean indicating if operation successful.
     */
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
     * @return ClauseVariable::variable_type indicating the system variable type.
     */
    ClauseVariable::variable_type determineVariableType(string w);

    /**
     * Parses the declarative portion of a query.
     * Populates the QueryObject with the new declaration(s).
     *
     * @return boolean if parsing was successful.
     */
    bool parseDeclarations();

    /**
     * Parses a set of declarations with the same type.
     *
     * @param type string for the shared declaration types.
     * @return boolean if parsing successful.
     */
    bool parseDeclarationsOfTypeString(string type);

    /**
     * Grammar check for a comma between declarations of same type.
     *
     * @return boolean if comma found.
     */
    bool lookForDeclarationComma();

    /**
     * Grammar check for a semicolon at end of declaration.
     *
     * @return boolean if semicolon found.
     */
    bool lookForDeclarationSemiColon();

    /**
     * Instantiates a QueryDeclaration object.
     * Populates the QueryObject with the new declaration.
     *
     * @param type string for the declaration's type.
     * @param synonym string for the declaration's synonym.
     */
    void generateDeclarationObject(string type, string synonym);

    /**
     * Parses the synonym that follows a "Select" command.
     * Populates the QueryObject with the select synonym.
     *
     * @return boolean if parsing was successful.
     */
    bool parseSelectTarget();

    /**
     * Parses a tuple of Selectable
     *
     * @return boolean if parsing successful.
     */
    bool parseSelectTuple();

    /**
     * Parses a single Selectable
     *
     * @return boolean if parsing successful.
     */
    bool parseSelectSingle();

    /**
     * Parses the attribute of the select target.
     *
     * @return attributeName of the parsed attribute.
     */
    Selectable::attributeName parseSelectAttribute();

    /**
     * Parses a group of such that clauses.
     * These clauses would be separated by "and" keyword in PQL.
     *
     * @return boolean if parsing successful.
     */
    bool parseSuchThatClauses();

    /**
     * Parses a group of pattern clauses.
     * These clauses would be separated by "and" keyword in PQL.
     *
     * @return boolean if parsing successful.
     */
    bool parsePatternClauses();

    /**
     * Parses a group of with clauses.
     * These clauses would be separated by "and" keyword in PQL.
     *
     * @return boolean if parsing successful.
     */
    bool parseWithClauses();

    /**
     * Parses a with clause.
     * Populates the query object with additional with clause.
     *
     * @return boolean if parsing successful.
     */
    bool parseWithClause();

    /**
     * Parses WithRef of either side of with clause.
     *
     * @return optional of a WithVariable
     */
    optional<WithVariable> parseWithRef();

    /**
     * Checks if a string is a valid attribute name.
     *
     * @param w string for the name.
     * @return boolean if name is valid.
     */
    bool isValidAttrName(string w);

    /**
     * Converts a string attribute name to
     * the corresponding enum.
     *
     * @param w string for the name.
     * @return attributeName enum.
     */
    WithVariable::attributeName toAttrName(string w);

    /**
     * Parses a clause of a query.
     *
     * @return boolean if parsing was successful.
     */
    bool parseClause();

    /**
     * Parses the type for the current clause being parsed.
     *
     * @return optional string representation of the clasue's type.
     */
    optional<string> parseClauseType();

    /**
     * Parses a clause variable for the current clause being parsed.
     *
     * @param clause string representation of the current clause.
     * @return optional string representation of the variable.
     */
    optional<string> parseClauseVariable(string clause);

    /**
     * Checks if a given grammar symbol is found in the next token
     *
     * @param symbol string to look for.
     * @param notFoundMessage message to print when symbol is not found (debug)
     * @return boolean if symbol was found
     */
    bool lookForClauseGrammarSymbol(string symbol, string notFoundMessage);

    /**
     * Parses a "pattern" clause of a query
     * Populates the QueryObject with a new pattern clause instance.
     *
     * @return boolean if parsing was successful.
     */
    bool parsePatternClause();

    /**
    * Parses pattern synonym before parentheses.
    *
    * @return optional of QueryDeclaration representing the synonym.
    */
    optional<QueryDeclaration> parsePatternSyn();

    /**
    * Parses LHS of pattern command.
    *
    * @return optional of string for the pattern's LHS.
    */
    optional<string> parsePatternLHS();

    /**
    * Parses pattern parameters for RHS.
    * Calls more specific parsing methods.
    *
    * @return optional of vector<PatternVariable> for the parsed parameters.
    */
    optional<PatternVariable> parsePatternRHS();

    /**
    * Parses Assign pattern parameters.
    * Used when an Assign pattern command is detected.
    *
    * @return optional of vector<PatternVariable> for the parsed parameters.
    */
    optional<vector<PatternVariable>> parseAssignPatternParams();

    /**
    * Parses While pattern parameters.
    * Used when an While pattern command is detected.
    *
    * @return optional of vector<PatternVariable> for the parsed parameters.
    */
    optional<vector<PatternVariable>> parseWhilePatternParams();

    /**
    * Parses If pattern parameters.
    * Used when an If pattern command is detected.
    *
    * @return optional of vector<PatternVariable> for the parsed parameters.
    */
    optional<vector<PatternVariable>> parseIfPatternParams();

    /**
    * Parses dummy pattern parameters.
    * Used when an invalid pattern command is detected.
    *
    * @return optional of vector<PatternVariable> for the parsed parameters.
    */
    optional<vector<PatternVariable>> parseDummyPatternParams();

    /**
     * Populates the QueryObject with a new Pattern Clause instance.
     *
     * @param patternSyn QueryDeclaration of the pattern's synonym.
     * @param lhs string representing the clause's LHS.
     * @param rhs vector<PatternVariable> representing the clause's variable RHS.
     */
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
