#pragma once

#include <queue>
#include <algorithm>
#include <iterator>

#include "PKB/PKBClient.h"
#include "QPS/Cache/Cache.h"
#include "AdaptersUtils.h"

class AffectsKBAdapter {
private:
    const string ROOT_CFG = "0";
    PKBClient *pkb;
    Cache *cache;
    bool isAffectsGraphBuilt = false;
    unordered_set<string> cachedAssignStmts = {};
    unordered_set<string> cachedParentStmts = {};
    unordered_map<string, CFGNode *> stmtNumToNodeMap;
    unordered_set<string> affectings;
    unordered_set<string> affecteds;
    vector<pair<string, string>> affectingAffectedPairs;

    bool hasAffectsGraph(const string &stmt) const;

    bool hasAffectsGraph() const;

    void addAllStarting(CFGNode *node, queue<CFGNode *> &mainQ);

    void buildAffectsGraphForProc(CFGNode *start);

    void buildAffectsGraph();

    bool bfsBool(PKBClient *pkb, CFGNode *start, const string &modifiedVar, const string &end);

    unordered_set<string> bfsDown(PKBClient *pkb, const string &stmtNum, bool isAnyResult = false);

    unordered_set<string> bfsUp(PKBClient *pkb, const string &stmtNum, bool isAnyResult = false);

    const unordered_set<string> &getAllParentStmt();

    const unordered_set<string> &getAllAssignStmt();
public:
    explicit AffectsKBAdapter(PKBClient *pkb);

    ~AffectsKBAdapter();

    bool isAffects(const string &stmtNum1, const string &stmtNum2);

    unordered_set<string> getDirectAffectsBy(const string &stmtNum);

    unordered_set<string> getDirectAffecting(const string &stmtNum);

    unordered_set<string> getAllStmtAffectingOther();

    unordered_set<string> getAllStmtAffectedByOther();

    vector<pair<string, string>> getDirectAffectsAll();

    bool isAffectsT(const string &stmtNum1, const string &stmtNum2);

    unordered_set<string> getAffectsTBy(const string &stmtNum);

    unordered_set<string> getAffectingT(const string &stmtNum);

    unordered_set<string> getAllStmtAffectingTOther();

    unordered_set<string> getAllStmtAffectedTByOther();

    vector<pair<string, string>> getAffectsTAll();

    bool isAffectingSomeStmt(const string &stmtNum);

    bool isAffectedBySomeStmt(const string &stmtNum);

    bool hasSomeAffectsAll();
};