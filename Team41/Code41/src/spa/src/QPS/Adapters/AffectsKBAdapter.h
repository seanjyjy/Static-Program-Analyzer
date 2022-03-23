#pragma once

#include <queue>

#include "PKB/PKBClient.h"
#include "QPS/Cache/Cache.h"
#include "AdaptersUtils.h"

class AffectsKBAdapter {
private:
    PKBClient* pkb;
    Cache* cache;
    CFGNode* affectsGraph = new CFGNode("0");
    unordered_map<string, CFGNode *> stmtNumToNodeMap;
    unordered_set<string> affectings;
    unordered_set<string> affecteds;
    vector<pair<string, string>> affectingAffectedPairs;

    bool bfs(CFGNode* start, const string& modifiedVar, const string& end);
    void bfs(CFGNode* start, const string& modifiedVar, unordered_set<string>& affected);
    void bfs(CFGNode *start, unordered_set<string>& affectedVars, unordered_set<string> &affecting);

    bool hasAffectsGraph();
    void addAllStarting(CFGNode* node, queue<CFGNode *> &mainQ);
    CFGNode* buildAffectsGraphForProc(CFGNode* start);
    void buildAffectsGraph();

    bool isModifyStmt(const string& stmtNum);
public:
    explicit AffectsKBAdapter(PKBClient* pkb);
    ~AffectsKBAdapter();

    bool isAffects(const string& stmtNum1, const string& stmtNum2);
    unordered_set<string> getDirectAffectsBy(const string& stmtNum);
    unordered_set<string> getDirectAffecting(const string& stmtNum);
    unordered_set<string> getAllStmtAffectingOther();
    unordered_set<string> getAllStmtAffectedByOther();
    vector<pair<string, string>> getDirectAffectsAll();

    bool isAffectsT(const string& stmtNum1, const string& stmtNum2);
    unordered_set<string> getAffectsTBy(const string& stmtNum);
    unordered_set<string> getAffectingT(const string& stmtNum);
    unordered_set<string> getAllStmtAffectingTOther();
    unordered_set<string> getAllStmtAffectedTByOther();
    vector<pair<string, string>> getAffectsTAll();
};