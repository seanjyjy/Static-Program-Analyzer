#include "AdaptersUtils.h"

void AdaptersUtils::runBFS(bool isForward, const CacheCallback &cacheAndContinue, const TerminateCheck &canTerminate,
                           Cache *cache, CFGNode *startNode) {

    if (startNode == nullptr) return;

    queue<CFGNode *> bfsQueue;
    unordered_set<string> visited;
    bfsQueue.push(startNode);

    while (!bfsQueue.empty()) {
        CFGNode *currNode = bfsQueue.front();
        vector<CFGNode *> nextNodes = isForward ? currNode->getChildren() : currNode->getParent();
        bfsQueue.pop();

        for (auto next: nextNodes) {
            string nextStmtNum = next->getStmtNum();
            if (visited.find(nextStmtNum) == visited.end()) {
                // check cache
                if (cacheAndContinue(nextStmtNum, cache)) {
                    bfsQueue.push(next);
                }
                if (canTerminate(nextStmtNum, cache)) {
                    return;
                };
                visited.insert(nextStmtNum);
            }
        }
    }
}

void AdaptersUtils::addFullMapping(const string &start, const string &end, Cache *cache) {
    cache->registerBooleanMapping(start, end);
    // Add forward Mapping
    cache->registerForwardMapping(start, end);
    // Add backwardMapping
    cache->registerBackwardMapping(end, start);
}

void AdaptersUtils::runBoolBFS(const string &start, const string &end, Cache *cache, CFGNode *node) {

    CacheCallback saveToCache = [start](const string &next, Cache *cache) {
        cache->registerBooleanMapping(start, next);
        return true;
    };

    TerminateCheck canEnd = [start, end](const string &next, Cache *cache) {
        if (cache->getBooleanMapping(next, end)) {
            cache->registerBooleanMapping(end, end);
            return true;
        }
        unordered_set<string> forwardCache = cache->getForwardMapping(next);
        if (!forwardCache.empty()) {
            bool hasNextT = forwardCache.find(end) != forwardCache.end();
            if (hasNextT) {
                cache->registerBooleanMapping(start, end);
            }
            return hasNextT;
        }
        return false;
    };

    runBFS(true, saveToCache, canEnd, cache, node);
}

void AdaptersUtils::runDownBFS(const string &stmtNum, Cache *cache, CFGNode *node) {
    CacheCallback saveToCache = [stmtNum](const string &next, Cache *cache) {
        unordered_set<string> forwardCache = cache->getForwardMapping(next);
        bool canUseCache = !forwardCache.empty() && stmtNum != next;

        cache->registerBooleanMapping(stmtNum, next);
        cache->registerForwardMapping(stmtNum, next);

        if (canUseCache) {
            for (auto &savedNext: forwardCache) {
                cache->registerBooleanMapping(stmtNum, savedNext);
                cache->registerForwardMapping(stmtNum, savedNext);
            }
            return false;
        }
        return true;
    };

    TerminateCheck canEnd = [](const string &, Cache *) { return false; };

    runBFS(true, saveToCache, canEnd, cache, node);
}

void AdaptersUtils::runUpBFS(const string &stmtNum, Cache *cache, CFGNode *node) {
    CacheCallback saveToCache = [stmtNum](const string &next, Cache *cache) {
        unordered_set<string> backwardCache = cache->getBackwardMapping(next);
        bool canUseCache = !backwardCache.empty() && stmtNum != next;

        cache->registerBooleanMapping(next, stmtNum);
        cache->registerBackwardMapping(stmtNum, next);

        if (canUseCache) {
            for (auto &savedNext: backwardCache) {
                cache->registerBooleanMapping(savedNext, stmtNum);
                cache->registerBackwardMapping(stmtNum, savedNext);
            }
            return false;
        }
        return true;
    };

    TerminateCheck canEnd = [](const string &, Cache *) { return false; };

    runBFS(false, saveToCache, canEnd, cache, node);
}
