#pragma once

#include "PKB/Relations/ManyToMany.h"
#include "Common/CFGNode.h"
#include <string>

using namespace std;
using StmtToNodeMap = unordered_map<string, CFGNode *>;

/**
 * A relational table that stores the Next abstractions
 */
class NextTable {
private:
    const string ROOT_STMT = "0";
    CFGNode *rootCFG;
    StmtToNodeMap stmtToNodeMap;
    vector<string> allAfterStmts; // set of stmts that can be executed after some stmts
    vector<string> allBeforeStmts; // set of stmts that can be executed before some stmts
    vector<pair<string, string>> allPairs; // pairs of stmts where stmt1 are executed directly after stmt2

public:
    NextTable();

    /**
     * Set CFG node and stmtToNodeMap
     */
    void setCFGNode(CFGNode *rootCFG, const StmtToNodeMap &stmtToNodeMap);

    bool isNext(const string &stmt1, const string &stmt2); // Checks if stmt2 can be executed immediately after stmt1
    vector<CFGNode *> getNextNodes(const string &stmt); // Get the stmts that are executed directly after stmt
    vector<CFGNode *> getPrevNodes(const string &stmt); // Gets the stmts that are executed directly before stmt
    vector<pair<string, string>> getAllNext(); // Gets stmts pair where stmt1 are executed directly after stmt2
    vector<string> getAllStmtsExecBeforeSomeStmt(); // Get list of stmts that are executed before some stmt
    vector<string> getAllStmtsExecAfterSomeStmt(); // Get list of stmts that are executed after some stmt
    size_t getNumOfStartNodes(); // Get the number of stmts that are executed before some stmt
    size_t getNumOfEndNodes(); // Get the number of stmts that are executed after some stmt
};

