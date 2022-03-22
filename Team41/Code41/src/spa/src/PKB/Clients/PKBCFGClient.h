#pragma once

#include <vector>
#include "Common/CFGNode.h"
#include "PKB/Tables/NextTable.h"

using namespace std;

/**
 * Used for retrieving pattern information for SIMPLE source code.
 *
 * This PKB facade class provides methods to
 * access the information stored in various relational pattern table.
 */
class PKBCFGClient {
protected:
    NextTable *nextTable;
    CFGNode *rootCFG;
    StmtToNodeMap stmtToNodeMap;

public:
    PKBCFGClient();

    ~PKBCFGClient();

    //=========================================== CFG =======================================================

    CFGNode *getCFGForStmt(const string &stmtNum);

    CFGNode *getRootCFG();

    //=========================================== Next ===================================================

    bool isNext(string stmt1, string stmt2) const; // Checks if stmt2 can be executed immediately after stmt1
    [[nodiscard]] vector<CFGNode *>
    getNextNodes(string stmt) const; // Get the nodes that are executed directly after stmt
    [[nodiscard]] vector<CFGNode *>
    getPrevNodes(string stmt) const; // Gets the nodes that are executed directly before stmt
    [[nodiscard]] vector<pair<string, string>>
    getAllNext() const; // Gets stmts pair where stmt1 are executed directly after stmt2
    [[nodiscard]] vector<string>
    getAllStmtsExecBeforeSomeStmt() const; // get list of stmts that are executed before some stmt
    [[nodiscard]] vector<string>
    getAllStmtsExecAfterSomeStmt() const; // get list of stmts that are executed after some stmt
    size_t getNumOfStartNodes() const; // Get the number of stmts that are executed before some stmt
    size_t getNumOfEndNodes() const; // Get the number of stmts that are executed after some stmt
};