#pragma once

#include <string>
#include <unordered_set>
#include <queue>

#include "PKB/PKBClient.h"
#include "Cache/Cache.h"

class NextKBAdapter {
private:
    PKBClient* pkb;
    Cache* cache;

    vector<string> dfsWrapper(string &start, string &end);
    void dfs(CFGNode *node, vector<string> &path, unordered_set<string> &visited, string &end);

    void addRelation(const string& start, const string& end);
public:
    explicit NextKBAdapter(PKBClient* pkb);
    //=========================================== Next ===================================================

    bool isNext(string stmt1, string stmt2) const; // Checks if stmt2 is executed after stmt1
    unordered_set<string> getAllStmtsNext(string stmtNum) const; // Gets the stmt that is next of this specified stmt
    unordered_set<string> getAllStmtsBefore(string stmtNum) const; // Gets the stmt that is before this specified stmt
    vector<pair<string, string>> getAllNext() const;
    unordered_set<string> getAllStmtsThatHaveNextStmt() const;
    unordered_set<string> getAllStmtsThatIsNextOfSomeStmt() const;

    bool isNextT(string stmt1, string stmt2) const;
    unordered_set<string> getAllStmtsNextT(string stmtNum) const; // Gets all stmt that is next of this specified stmt
    unordered_set<string> getAllStmtsTBefore(string stmtNum) const; // Gets all stmt that is before of this specified stmt
    vector<pair<string, string>> getAllNextT() const;
    unordered_set<string> getAllStmtsThatHaveNextTStmt() const;
    unordered_set<string> getAllStmtThatIsNextTOfSomeStmt() const;
};