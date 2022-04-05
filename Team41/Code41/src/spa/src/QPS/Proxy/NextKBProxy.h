#pragma once

#include <string>
#include <unordered_set>
#include <queue>

#include "PKB/PKBClient.h"
#include "QPS/Cache/Cache.h"
#include "Common/CFGNode.h"
#include "ProxyUtils.h"

class NextKBProxy {
private:
    PKBClient *pkb;
    Cache *cache;
public:
    explicit NextKBProxy(PKBClient *pkb);

    ~NextKBProxy();

    /**
     * Checks if stmt2 can be executed immediately after stmt1.
     *
     * @param stmt1 A stmt number.
     * @param stmt2 A stmt number.
     * @return True if stmt2 can be executed immediately after stmt1 else false.
     */
    [[nodiscard]] bool isNext(string stmt1, string stmt2) const;

    /**
     * Get a vector of CFGNodes that represents stmts that are executed directly after the provided stmtNum.
     *
     * @param stmtNum Starting stmtNum.
     * @return A vector of CFGNodes that is executed directly next of provided stmtNum.
     */
    [[nodiscard]] vector<CFGNode *> getNextNodes(string stmtNum) const;

    /**
     * Get a vector of CFGNodes that represents stmts that are executed directly before the provided stmtNum.
     *
     * @param stmtNum Ending stmtNum.
     * @return A vector of CFGNodes that is executed directly before of provided stmtNum.
     */
    [[nodiscard]] vector<CFGNode *> getPrevNodes(string stmtNum) const;

    /**
     * @return Stmts pair where stmt1 are executed directly after stmt2
     */
    [[nodiscard]] vector<pair<string, string>> getAllNext() const;

    /**
     * @return Vector of stmts that are executed before some stmt
     */
    [[nodiscard]] vector<string> getAllStmtsThatHaveNextStmt() const;

    /**
     * @return Vector of stmts that are executed after some stmt
     */
    [[nodiscard]] vector<string> getAllStmtsThatIsNextOfSomeStmt() const;

    /**
     * Checks if stmt2 can be executed after stmt1.
     *
     * @param stmt1 A stmt number.
     * @param stmt2 A stmt number.
     * @return True if stmt2 can be executed after stmt1 else false.
     */
    bool isNextT(const string &stmt1, const string &stmt2);

    /**
     * Get the stmts that are executed after the provided stmtNum.
     *
     * @param stmtNum Starting stmtNum.
     * @return A set of stmts that is executed after the provided stmtNum.
     */
    unordered_set<string> getAllStmtsNextT(const string &stmtNum);

    /**
     * Gets the stmts that are executed before the provided stmtNum stmt.
     *
     * @param stmtNum Ending stmtNum.
     * @return A set of stmts that is executed before of provided stmtNum.
     */
    unordered_set<string> getAllStmtsTBefore(const string &stmtNum);

    /**
     * @return Stmts pair where stmt1 are executed after stmt2
     */
    vector<pair<string, string>> getAllNextT();

    /**
     * @return Vector of stmts that are executed before some stmt
     */
    vector<string> getAllStmtsThatHaveNextTStmt() const;

    /**
     * @return Vector of stmts that are executed after some stmt
     */
    vector<string> getAllStmtsThatIsNextTOfSomeStmt() const;
};