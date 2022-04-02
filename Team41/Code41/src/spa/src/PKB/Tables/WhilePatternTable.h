#pragma once

#include <unordered_map>
#include "PKB/Relations/ManyToMany.h"

/**
 * A relational table that stores the Pattern abstractions for while statements
 * - While pattern relation maps each whileStmt to the varName used in condition block
 */
class WhilePatternTable {
private:
    // whileStmt <--> var
    ManyToMany<string, string> patternRelation;

public:
    WhilePatternTable();

    /**
     * Set the provided while pattern into the table
     *
     * @param stmtNum while statement
     * @param predVariable the variable in the condition block
     */
    void setPattern(const string &stmtNum, const string &predVariable);

    /**
     * Gets all whileStmts that has the variable in the condition block
     *
     * @param varName the variable in the condition block
     * @return the set of while statements that uses the varName in condition block
     */
    unordered_set<string> getStmtFromVar(const string &varName);

    /**
     * Gets all whileStmt-variable pair
     *
     * @return list of whileStmt-variable pair
     */
    vector<pair<string, string>> getStmtNVar();

    /**
     * Gets all whileStmts that has a variable in the condition block
     *
     * @return the set of while statements that uses any variable in condition
     */
    unordered_set<string> getStmtsUsingSomeVarInPred();
};

