#pragma once

#include <functional>
#include <queue>

#include "QPS/Cache/Cache.h"
#include "Common/CFGNode.h"
#include "PKB/PKBClient.h"

typedef std::function<bool(const string &)> CacheCallback;
typedef std::function<bool(CFGNode *)> TerminateCheck;

class AdaptersUtils {
private:
public:
    static void runBFS(bool isForward, const CacheCallback &cacheAndContinue, const TerminateCheck &canTerminate,
                       CFGNode *startNode);

    static void runBoolBFS(const string &start, const string &end, Cache *cache, CFGNode *node);

    static void runDownBFS(const string &stmtNum, Cache *cache, CFGNode *node);

    static void runUpBFS(const string &stmtNum, Cache *cache, CFGNode *node);

    static void fullBFS(Cache *cache, CFGNode *node);

    static CFGNode *getStartingParentNode(CFGNode *rootCFG, const string &stmt);
};