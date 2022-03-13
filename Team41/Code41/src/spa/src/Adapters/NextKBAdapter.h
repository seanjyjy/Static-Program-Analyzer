#pragma once

#include <string>
#include <unordered_set>

#include "PKB/PKBClient.h"
#include "Cache/Cache.h"

class NextKBAdapter {
private:
    PKBClient* pkb;
    Cache* cache;
public:
    explicit NextKBAdapter(PKBClient* pkb);
    //=========================================== Next ===================================================

    bool isNext(string stmt1, string stmt2) const; // Checks if stmt2 is executed after stmt1
    string getStmtNext(string stmtNum) const; // Gets the stmt that is next of this specified stmt
    string getStmtBefore(string stmtNum) const; // Gets the stmt that is before this specified stmt
    vector<pair<string, string>> getAllNext() const;
    unordered_set<string> getAllStmtsThatHaveNextStmt() const;
    unordered_set<string> getAllStmtsThatIsNextOfSomeStmt() const;

    bool isNextT(string stmt1, string stmt2) const;
    unordered_set<string> getAllStmtNextT(string stmtNum) const; // Gets all stmt that is next of this specified stmt
    unordered_set<string> getAllStmtBefore(string stmtNum) const; // Gets all stmt that is before of this specified stmt
    vector<pair<string, string>> getAllNextT() const;
    unordered_set<string> getAllStmtsThatHaveNextTStmt() const;
    unordered_set<string> getAllStmtThatIsNextTOfSomeStmt() const;
};