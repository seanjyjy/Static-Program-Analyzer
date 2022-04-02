#pragma once

#include <vector>
#include <unordered_set>
#include "Common/TNode.h"

using namespace std;

class AssignPatternTable;

class WhilePatternTable;

class IfPatternTable;

/**
 * Used for retrieving pattern information for SIMPLE source code.
 *
 * This PKB facade class provides methods to
 * access the information stored in various relational pattern table.
 */
class PKBPatternClient {
protected:
    AssignPatternTable *assignPatternTable;
    WhilePatternTable *whilePatternTable;
    IfPatternTable *ifPatternTable;

public:
    PKBPatternClient();

    ~PKBPatternClient();

    //=========================================== Assign ===================================================

    /**
     * Gets list of assignStmt from pattern
     */
    unordered_set<string> getAssignStmtFromPattern(TNode *patternAST) const;

    /**
     * Gets list of assignStmt from pattern and varName
     */
    unordered_set<string> getAssignStmtFromPatternNVar(TNode *patternAST, const string &varName) const;

    /**
     * Gets list of assignStmt-varName from pattern
     */
    vector<pair<string, string>> getAssignStmtNVarFromPattern(TNode *patternAST) const;

    /**
     * Gets list of assignStmt from subpattern
     */
    unordered_set<string> getAssignStmtFromSubpattern(TNode *subPatternAST) const;

    /**
     * Gets list of assignStmt from subpattern and varName
     */
    unordered_set<string> getAssignStmtFromSubpatternNVar(TNode *subPatternAST, const string &varName) const;

    /**
     * Gets list of assignStmt-varName from subpattern
     */
    vector<pair<string, string>> getAssignStmtNVarFromSubpattern(TNode *subPatternAST) const;

    //=========================================== If =======================================================

    /**
     * Gets list of ifStmt from condition variable
     */
    unordered_set<string> getIfStmtUsingVarCond(const string &varName) const;

    /**
     * Gets list of ifStmt-variable where ifStmt uses variable in condition
     */
    vector<pair<string, string>> getIfStmtNVarPairs() const;

    /**
     * Gets ifStmts using some variable in condition block
     */
    unordered_set<string> getIfStmtUsingSomeVarCond() const;

    //=========================================== If =======================================================

    /**
     * Gets list of whileStmt from condition variable
     */
    unordered_set<string> getWhileStmtUsingVarCond(const string &varName) const;

    /**
     * Gets list of whileStmt-variable where whileStmt uses variable in condition
     */
    vector<pair<string, string>> getWhileStmtNVarPairs() const;

    /**
     * Gets whileStmts using some variable in condition block
     */
    unordered_set<string> getWhileStmtUsingSomeVarCond() const;
};