#pragma once

#include <queue>

#include "PKB/PKBClient.h"
#include "QPS/Cache/Cache.h"

class AffectsKBAdapter {
private:
    PKBClient* pkb;
    Cache* cache;
    bool bfs(CFGNode* start, const string& modifiedVar, const string& end);
    void bfs(CFGNode* start, const string& modifiedVar, unordered_set<string>& affected);
    void bfs(CFGNode *start, unordered_set<string>& affectedVars, unordered_set<string> &affecting);

    bool isModifyStmt(const string& stmtNum);
public:
    explicit AffectsKBAdapter(PKBClient* pkb);
    ~AffectsKBAdapter();

    bool isAffects(const string& stmt1, const string& stmt2);
    unordered_set<string> getDirectAffectsBy(const string& stmt);
    unordered_set<string> getDirectAffecting(const string& stmt);
    unordered_set<string> getAllStmtAffectingOther();
    unordered_set<string> getAllStmtAffectedByOther();
    vector<pair<string, string>> getDirectAffectsAll();
};