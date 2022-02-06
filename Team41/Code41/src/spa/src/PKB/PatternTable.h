#pragma once

#include <unordered_map>
#include <algorithm>
#include "ManyToMany.h"
#include "SingleMap.h"
#include "Common/TNode.h"
#include "Common/TreeUtils.h"

class PatternTable {
private:
    // rhs full pattern -> (assignStmt <--> var)
    SingleMap<string, ManyToMany<string, string>> patternRelation;

public:
    PatternTable();

    void setPattern(string stmtNum, string lhsVariable, TNode *rhsAssignAST);

    unordered_set<string> getAllStmtsFromFullPattern(TNode *patternAST);
    unordered_set<string> getStmtFromFullPatternNVar(TNode *patternAST, string varName);
    /**
     * Get assignStmt-variable pair based on full pattern
     * @param patternAST the full AST
     * @return assignStmt-variable pair
     */
    vector<pair<string, string>> getStmtNVarFromFullPattern(TNode *patternAST);

    unordered_set<string> getAllStmtsFromSubPattern(TNode *subPatternAST);
    unordered_set<string> getStmtFromSubPatternNVar(TNode *subPatternAST, string varName);
    /**
     * Get assignStmt-variable pair based on sub pattern
     * @param subPatternAST the sub AST
     * @return assignStmt-variable pair
     */
    vector<pair<string, string>> getStmtNVarFromSubPattern(TNode *subPatternAST);
};

