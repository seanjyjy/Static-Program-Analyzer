#pragma once

#include <string>
#include <unordered_set>
#include <queue>
#include <functional>

#include "PKB/PKBClient.h"
#include "QPS/Cache/Cache.h"
#include "Common/CFGNode.h"

//typedef bool (*CacheCallback)(Cache* cache, const string& next);
typedef std::function<bool (const string&, Cache* cache)> CacheCallback;
typedef std::function<bool (const string&, Cache* cache)> TerminateCheck;

class NextKBAdapter {
private:
    const string ROOT_STMT = "0";
    PKBClient* pkb;
    Cache* cache;

    void fullBFS();
    void runBFS(const string &start, const string &end, bool isForward, const CacheCallback& cacheAndContinue,
                const TerminateCheck& canTerminate);

    void addFullMapping(const string& start, const string& end);
public:
    explicit NextKBAdapter(PKBClient* pkb);
    ~NextKBAdapter();
    //=========================================== Next ===================================================

    bool isNext(string stmt1, string stmt2) const; // Checks if stmt2 can be executed immediately after stmt1
    vector<CFGNode *> getNextNodes(string stmtNum) const; // Get the stmts that are executed directly after stmt
    vector<CFGNode *> getPrevNodes(string stmtNum) const; // Gets the stmts that are executed directly before stmt
    vector<pair<string, string>> getAllNext() const; // Gets stmts pair where stmt1 are executed directly after stmt2
    vector<string> getAllStmtsThatHaveNextStmt() const; // Get list of stmts that are executed before some stmt
    vector<string> getAllStmtsThatIsNextOfSomeStmt() const; // Get list of stmts that are executed after some stmt

    bool isNextT(const string& stmt1, const string& stmt2); //  Checks if stmt2 can be executed after stmt1
    unordered_set<string> getAllStmtsNextT(const string& stmtNum); // Get the stmts that are executed after stmt
    unordered_set<string> getAllStmtsTBefore(const string& stmtNum); // Gets the stmts that are executed before stmt
    vector<pair<string, string>> getAllNextT(); // Gets stmts pair where stmt1 are executed after stmt2
    vector<string> getAllStmtsThatHaveNextTStmt(); // Get list of stmts that are executed before some stmt
    vector<string> getAllStmtsThatIsNextTOfSomeStmt(); // Get list of stmts that are executed after some stmt
};