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

    // ============================================= Affects ========================================================

    /**
     * Checks if stmtNum1 affects stmtNum2.
     *
     * @param stmtNum1 A statement number.
     * @param stmtNum2 A statement number.
     * @return True if stmtNum1 directly affects StmtNum2 else false.
     */
    bool isAffects(const string &stmtNum1, const string &stmtNum2);

    /**
     * Get the stmts that are directly affected by the provided stmtNum.
     *
     * @param stmtNum Starting statement number.
     * @return A set of stmts that is directly affected by the provided stmtNum.
     */
    unordered_set<string> getDirectAffectsBy(const string &stmtNum);

    /**
     * Gets the stmts that are directly affecting the provided stmtNum stmt.
     *
     * @param stmtNum Ending statement number.
     * @return A set of stmts that is directly affecting the provided stmtNum.
     */
    unordered_set<string> getDirectAffecting(const string &stmtNum);

    /**
     * @return A set of stmts that are directly affecting some stmt
     */
    unordered_set<string> getAllStmtAffectingOther();

    /**
     * @return A set of stmts that are directly affected by some stmt
     */
    unordered_set<string> getAllStmtAffectedByOther();

    /**
     * @return Stmts pair where stmt1 are directly affecting stmt2
     */
    vector<pair<string, string>> getDirectAffectsAll();

    // ============================================= AffectsT =======================================================

    /**
     * Checks if stmtNum1 affects stmtNum2.
     *
     * @param stmtNum1 A statement number.
     * @param stmtNum2 A statement number.
     * @return True if stmtNum1 directly or indirectly affects StmtNum2 else false.
     */
    bool isAffectsT(const string &stmtNum1, const string &stmtNum2);

    /**
     * Get the stmts that are directly or indirectly affected by the provided stmtNum.
     *
     * @param stmtNum Starting statement number.
     * @return A set of stmts that is directly or indirectly affected by the provided stmtNum.
     */
    unordered_set<string> getAffectsTBy(const string &stmtNum);

    /**
      * Gets the stmts that are directly or indirectly affecting the provided stmtNum stmt.
      *
      * @param stmtNum Ending statement number.
      * @return A set of stmts that is directly or indirectly affecting the provided stmtNum.
      */
    unordered_set<string> getAffectingT(const string &stmtNum);

    /**
     * @return A set of stmts that are directly or indirectly affecting some stmt
     */
    unordered_set<string> getAllStmtAffectingTOther();

    /**
     * @return A set of stmts that are directly or indirectly affected by some stmt
     */
    unordered_set<string> getAllStmtAffectedTByOther();

    /**
     * @return Stmts pair where stmt1 are directly or indirectly affecting stmt2
     */
    vector<pair<string, string>> getAffectsTAll();

    /**
     * Checks if the given stmtNum affects some stmt.
     *
     * @param stmtNum A statement number.
     * @return True if given stmtNum affects some stmt else false.
     */
    bool isAffectingSomeStmt(const string &stmtNum);

    /**
     * Checks if the given stmtNum is affected by some stmt.
     *
     * @param stmtNum A statement number.
     * @return True if given stmtNum is affected by some stmt else false.
     */
    bool isAffectedBySomeStmt(const string &stmtNum);

    /**
     * @return True if there is any affects relation else false
     */
    bool hasSomeAffectsAll();
};