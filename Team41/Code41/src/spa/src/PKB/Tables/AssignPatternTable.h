#pragma once

#include <unordered_map>
#include <algorithm>
#include "PKB/Relations/ManyToMany.h"
#include "PKB/Relations/SingleMap.h"
#include "Common/TNode.h"
#include "Common/TreeUtils.h"

/**
 * A relational table that stores the Pattern abstractions for assign statements
 * - Assign pattern relation maps each expression pattern to a Many-Many relationship mapping
 */
class AssignPatternTable {
private:
    // rhs full pattern -> (assignStmt <--> var)
    SingleMap<string, ManyToMany<string, string>> patternRelation;

public:
    AssignPatternTable();

    /**
     * Set the provided assignment pattern into the table
     *
     * @param stmtNum assign statement
     * @param lhsVariable the modified variable in the statement
     * @param assignAST the AST for the RHS expression of the statement
     */
    void setPattern(const string &stmtNum, const string &lhsVariable, TNode *rhsAssignAST);

    /**
     * Gets all assignStmts that matches the provided full pattern
     *
     * @param patternAST the AST for the RHS full assignment expression pattern
     * @return the set of assignment statements that matches the pattern
     */
    unordered_set<string> getAllStmtsFromFullPattern(TNode *patternAST);

    /**
     * Gets all assignStmts that matches the provided full pattern
     *
     * @param patternAST the AST for the RHS full assignment expression pattern
     * @param varName the LHS variable name
     * @return the set of assignment statements that matches the pattern and modifies the variable
     */
    unordered_set<string> getStmtFromFullPatternNVar(TNode *patternAST, const string &varName);

    /**
     * Get assignStmt-variable pair based on full pattern
     *
     * @param patternAST the full AST pattern
     * @return list of assignStmt-variable pair
     */
    vector<pair<string, string>> getStmtNVarFromFullPattern(TNode *patternAST);

    /**
     * Gets all assignStmts that partially/fully matches the provided sub pattern
     *
     * @param subPatternAST the AST for the RHS assignment expression subpattern
     * @return the set of assignment statements that partially/fully matches the sub pattern
     */
    unordered_set<string> getAllStmtsFromSubPattern(TNode *subPatternAST);

    /**
     * Gets all assignStmts that matches the provided full pattern
     *
     * @param subPatternAST the AST for the RHS assignment expression subpattern
     * @param varName the LHS variable name
     * @return the set of assignment statements that matches the subpattern and modifies the variable
     */
    unordered_set<string> getStmtFromSubPatternNVar(TNode *subPatternAST, const string &varName);

    /**
     * Get assignStmt-variable pair based on sub pattern
     *
     * @param subPatternAST the AST for the RHS assignment expression subpattern
     * @return list of assignStmt-variable pair
     */
    vector<pair<string, string>> getStmtNVarFromSubPattern(TNode *subPatternAST);
};

