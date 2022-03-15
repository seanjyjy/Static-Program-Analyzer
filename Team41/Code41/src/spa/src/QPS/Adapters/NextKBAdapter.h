#pragma once

#include <string>
#include <unordered_set>
#include <queue>

#include "PKB/PKBClient.h"
#include "QPS/Cache/Cache.h"
#include "Common/CFGNode.h"

class NextKBAdapter {
private:
    PKBClient* pkb;
    Cache* cache;

    void booleanBFS(string &start, string &end);
    void forwardBFS(string &start);
    void backwardBFS(string &start);
    void fullBFS();

    void addForwardMapping(const string &currStmtNum, unordered_set<string> &mapping);
    void addBackwardMapping(const string &currStmtNum, unordered_set<string> &mapping);

    void addBooleanRelation(const string& start, const string& end);
    void addForwardRelation(const string& start, const string& end);
    void addBackwardRelation(const string& start, const string& end);
public:
    explicit NextKBAdapter(PKBClient* pkb);
    //=========================================== Next ===================================================

    bool isNext(string stmt1, string stmt2) const; // Checks if stmt2 can be executed immediately after stmt1
    vector<CFGNode *> getNextNodes(string stmtNum) const; // Get the stmts that are executed directly after stmt
    vector<CFGNode *> getPrevNodes(string stmtNum) const; // Gets the stmts that are executed directly before stmt
    vector<pair<string, string>> getAllNext() const; // Gets stmts pair where stmt1 are executed directly after stmt2
    vector<string> getAllStmtsThatHaveNextStmt() const; // Get list of stmts that are executed before some stmt
    vector<string> getAllStmtsThatIsNextOfSomeStmt() const; // Get list of stmts that are executed after some stmt

    bool isNextT(string stmt1, string stmt2); //  Checks if stmt2 can be executed after stmt1
    unordered_set<string> getAllStmtsNextT(string stmtNum); // Get the stmts that are executed after stmt
    unordered_set<string> getAllStmtsTBefore(string stmtNum); // Gets the stmts that are executed before stmt
    vector<pair<string, string>> getAllNextT(); // Gets stmts pair where stmt1 are executed after stmt2
    vector<string> getAllStmtsThatHaveNextTStmt(); // Get list of stmts that are executed before some stmt
    vector<string> getAllStmtsThatIsNextTOfSomeStmt(); // Get list of stmts that are executed after some stmt
};