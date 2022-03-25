#pragma once

#include <functional>
#include <queue>

#include "QPS/Cache/Cache.h"
#include "Common/CFGNode.h"
#include "PKB/PKBClient.h"

typedef std::function<bool(const string &, Cache *cache)> CacheCallback;
typedef std::function<bool(const string &, Cache *cache)> TerminateCheck;

class AdaptersUtils {
private:
    static void runBFS(bool isForward, const CacheCallback &cacheAndContinue, const TerminateCheck &canTerminate,
                       Cache *cache, CFGNode *startNode);

public:
    static void addFullMapping(const string &start, const string &end, Cache *cache);

    static void runBoolBFS(const string &start, const string &end, Cache *cache, CFGNode *node);

    static void runDownBFS(const string &stmtNum, Cache *cache, CFGNode *node);

    static void runUpBFS(const string &stmtNum, Cache *cache, CFGNode *node);
};