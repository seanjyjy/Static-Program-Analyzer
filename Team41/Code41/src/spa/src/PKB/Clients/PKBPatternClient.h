#pragma once

#include <vector>
#include "PKBBaseClient.h"
#include "Common/TNode.h"

using namespace std;

class UsesTable;
class ModifiesTable;
class FollowsTable;
class CallsTable;
class ParentTable;
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

    unordered_set<string> getAssignStmtFromPattern(TNode *patternAST) const;// Gets list of assignStmt from pattern
    unordered_set<string> getAssignStmtFromPatternNVar(TNode *patternAST, string varName) const;// Gets list of assignStmt from pattern and varName
    vector<pair<string, string>> getAssignStmtNVarFromPattern(TNode *patternAST) const;// Gets list of assignStmt-varName from pattern

    unordered_set<string> getAssignStmtFromSubpattern(TNode *subPatternAST) const;// Gets list of assignStmt from subpattern
    unordered_set<string> getAssignStmtFromSubpatternNVar(TNode *subPatternAST, const string& varName) const;// Gets list of assignStmt from subpattern and varName
    vector<pair<string, string>> getAssignStmtNVarFromSubpattern(TNode *subPatternAST) const;// Gets list of assignStmt-varName from subpattern

    //=========================================== If =======================================================

    unordered_set<string> getIfStmtUsingVarCond(string varName) const;// Gets list of ifStmt from condition variable
    vector<pair<string, string>> getIfStmtNVarPairs() const;// Gets list of ifStmt-variable where ifStmt uses variable in condition
    unordered_set<string> getIfStmtUsingSomeVarCond() const;// Gets ifStmts using some variable in condition block

    //=========================================== If =======================================================

    unordered_set<string> getWhileStmtUsingVarCond(string varName) const;// Gets list of whileStmt from condition variable
    vector<pair<string, string>> getWhileStmtNVarPairs() const;// Gets list of whileStmt-variable where whileStmt uses variable in condition
    unordered_set<string> getWhileStmtUsingSomeVarCond() const;// Gets whileStmts using some variable in condition block
};