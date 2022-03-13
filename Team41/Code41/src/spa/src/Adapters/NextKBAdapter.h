#pragma once

#include <string>
#include <unordered_set>
#include <queue>

#include "PKB/PKBClient.h"
#include "Cache/Cache.h"
#include "Common/CFGNode.h"

class NextKBAdapter {
private:
    PKBClient* pkb;
    Cache* cache;

    void booleanBFSWrapper(string &start, string &end);
    void booleanBFS(unordered_set<string> &visited, string &end, string& currStmtNum, queue<CFGNode *> &bfsQueue);

    void forwardDFSWrapper(string &start);
    void forwardDFS(CFGNode *node, unordered_set<string> &visited);

    void backwardDFSWrapper(string &end);
    void backwardDFS(CFGNode *node, unordered_set<string> &visited);

    void addBooleanRelation(const string& start, const string& end);
    void addForwardRelation(const string& start, const string& end);
    void addBackwardRelation(const string& start, const string& end);
public:
    explicit NextKBAdapter(PKBClient* pkb);
    //=========================================== Next ===================================================

    bool isNext(string stmt1, string stmt2) const; // Checks if stmt2 is executed after stmt1
    vector<CFGNode *> getNextNodes(string stmtNum) const; // Gets the stmt that is next of this specified stmt
    vector<CFGNode *> getPrevNodes(string stmtNum) const; // Gets the stmt that is before this specified stmt
    vector<pair<string, string>> getAllNext() const;
    vector<string> getAllStmtsThatHaveNextStmt() const;
    vector<string> getAllStmtsThatIsNextOfSomeStmt() const;

    bool isNextT(string stmt1, string stmt2);
    unordered_set<string> getAllStmtsNextT(string stmtNum); // Gets all stmt that is next of this specified stmt
    unordered_set<string> getAllStmtsTBefore(string stmtNum); // Gets all stmt that is before of this specified stmt
    vector<pair<string, string>> getAllNextT();
    unordered_set<string> getAllStmtsThatHaveNextTStmt();
    unordered_set<string> getAllStmtThatIsNextTOfSomeStmt();
};