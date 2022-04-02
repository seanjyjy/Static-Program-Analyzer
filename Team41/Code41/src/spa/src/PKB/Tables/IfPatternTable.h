#pragma once

#include <unordered_map>
#include "PKB/Relations/ManyToMany.h"

/**
 * A relational table that stores the Pattern abstractions for if statements
 * - If pattern relation maps each ifStmt to the varName used in condition block
 */
class IfPatternTable {
private:
    // (ifStmt <--> var)
    ManyToMany<string, string> patternRelation;

public:
    IfPatternTable();

    /**
     * Set the provided if pattern into the table
     *
     * @param stmtNum if statement
     * @param predVariable the variable in the condition block
     */
    void setPattern(const string &stmtNum, const string &predVariable);

    /**
     * Gets all ifStmts that has the variable in the condition block
     *
     * @param varName the variable in the condition block
     * @return the set of if statements that uses the variable in condition
     */
    unordered_set<string> getStmtFromVar(const string &varName);

    /**
     * Gets all ifStmt-variable pair
     *
     * @return list of ifStmt-variable pair
     */
    vector<pair<string, string>> getStmtNVar();

    /**
     * Get all ifStmts that has a variable in the condition block
     *
     * @return the set of if statements that uses any variable in condition
     */
    unordered_set<string> getStmtsUsingSomeVarInPred();
};

