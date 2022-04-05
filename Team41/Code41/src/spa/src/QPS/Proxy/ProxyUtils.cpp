#include "ProxyUtils.h"

void ProxyUtils::runBFS(bool isForward, CFGNode *startNode, const CacheCallback &cacheAndContinue,
                        const TerminateCheck &canTerminate) {

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
            if (visited.find(nextStmtNum) != visited.end()) {
                continue;
            }
            // check cache
            if (cacheAndContinue(nextStmtNum)) {
                bfsQueue.push(next);
            }
            if (canTerminate(next)) {
                return;
            };
            visited.insert(nextStmtNum);
        }
    }
}

void ProxyUtils::runBoolBFS(const string &start, const string &end, Cache *cache, CFGNode *node) {

    CacheCallback saveToCache = [&start, &cache](const string &next) {
        cache->registerBooleanMapping(start, next);
        return true;
    };

    TerminateCheck canEnd = [&start, &end, &cache](CFGNode *next) {
        string nextStmt = next->getStmtNum();
        if (cache->getBooleanMapping(nextStmt, end)) {
            cache->registerBooleanMapping(end, end);
            return true;
        }
        unordered_set<string> forwardCache = cache->getForwardMapping(nextStmt);
        if (forwardCache.empty()) {
            return false;
        }
        bool hasNextT = forwardCache.find(end) != forwardCache.end();
        if (hasNextT) {
            cache->registerBooleanMapping(start, end);
        }
        return hasNextT;
    };

    runBFS(true, node, saveToCache, canEnd);
}

void ProxyUtils::runDownBFS(const string &stmtNum, Cache *cache, CFGNode *node) {
    CacheCallback saveToCache = [&stmtNum, &cache](const string &next) {
        unordered_set<string> forwardCache = cache->getForwardMapping(next);
        bool canUseCache = !forwardCache.empty() && stmtNum != next;

        cache->registerBooleanMapping(stmtNum, next);
        cache->registerForwardMapping(stmtNum, next);

        if (!canUseCache) {
            return true;
        }
        for (auto &savedNext: forwardCache) {
            cache->registerForwardMapping(stmtNum, savedNext);
        }
        return false;
    };

    runBFS(true, node, saveToCache);
}

void ProxyUtils::runUpBFS(const string &stmtNum, Cache *cache, CFGNode *node) {
    CacheCallback saveToCache = [&stmtNum, &cache](const string &next) {
        unordered_set<string> backwardCache = cache->getBackwardMapping(next);
        bool canUseCache = !backwardCache.empty() && stmtNum != next;

        cache->registerBooleanMapping(next, stmtNum);
        cache->registerBackwardMapping(stmtNum, next);

        if (!canUseCache) {
            return true;
        }
        for (auto &savedNext: backwardCache) {
            cache->registerBooleanMapping(savedNext, stmtNum);
            cache->registerBackwardMapping(stmtNum, savedNext);
        }
        return false;
    };

    runBFS(false, node, saveToCache);
}

void ProxyUtils::fullBFS(Cache *cache, CFGNode *node) {
    unordered_set<CFGNode *> mainVisited;
    queue<CFGNode *> mainQ;

    // we cannot start with root its 0!
    const vector<CFGNode *> &procedures = node->getChildren();
    for (auto &startProc: procedures) {
        mainQ.push(startProc);
        mainVisited.insert(startProc);
    }

    while (!mainQ.empty()) {
        CFGNode *curr = mainQ.front();
        string currStmtNum = curr->getStmtNum();
        const vector<CFGNode *> &children = curr->getChildren();

        for (auto next: children) {
            if (mainVisited.find(next) == mainVisited.end()) {
                mainQ.push(next);
                mainVisited.insert(next);
            }
        }

        CacheCallback saveToCache = [&currStmtNum, &cache](const string &next) {
            cache->addAllMappingPair({currStmtNum, next});
            // Add forward Mapping
            cache->registerForwardMapping(currStmtNum, next);
            // Add backwardMapping
            cache->registerBackwardMapping(next, currStmtNum);

            return true;
        };

        runBFS(true, curr, saveToCache);
        mainQ.pop();
    }
}

CFGNode *ProxyUtils::getStartingParentNode(CFGNode *rootCFG, const string &stmt) {
    int stmtNum = stoi(stmt);

    const vector<CFGNode *> &children = rootCFG->getChildren();

    // Binary search for first element that is greater than stmt
    auto result = upper_bound(children.begin(), children.end(), stmtNum, [](int value, CFGNode *currNode) {
        return value < stoi(currNode->getStmtNum());
    });

    if (result == children.begin()) {
        return nullptr;
    }

    // Get the first stmt of procedure
    // Note: does not check if stmt is out of range
    return *(--result);
}
