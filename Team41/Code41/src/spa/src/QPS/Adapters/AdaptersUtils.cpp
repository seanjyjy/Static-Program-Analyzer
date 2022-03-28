#include "AdaptersUtils.h"

void AdaptersUtils::runBFS(bool isForward, const CacheCallback &cacheAndContinue, const TerminateCheck &canTerminate,
                           CFGNode *startNode) {

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
}

void AdaptersUtils::runBoolBFS(const string &start, const string &end, Cache *cache, CFGNode *node) {

    CacheCallback saveToCache = [start, &cache](const string &next) {
        cache->registerBooleanMapping(start, next);
        return true;
    };

    TerminateCheck canEnd = [start, end, &cache](CFGNode *next) {
        string nextStmt = next->getStmtNum();
        if (cache->getBooleanMapping(nextStmt, end)) {
            cache->registerBooleanMapping(end, end);
            return true;
        }
        unordered_set<string> forwardCache = cache->getForwardMapping(nextStmt);
        if (!forwardCache.empty()) {
            bool hasNextT = forwardCache.find(end) != forwardCache.end();
            if (hasNextT) {
                cache->registerBooleanMapping(start, end);
            }
            return hasNextT;
        }
        return false;
    };

    runBFS(true, saveToCache, canEnd, node);
}

void AdaptersUtils::runDownBFS(const string &stmtNum, Cache *cache, CFGNode *node) {
    CacheCallback saveToCache = [stmtNum, &cache](const string &next) {
        unordered_set<string> forwardCache = cache->getForwardMapping(next);
        bool canUseCache = !forwardCache.empty() && stmtNum != next;

        cache->registerBooleanMapping(stmtNum, next);
        cache->registerForwardMapping(stmtNum, next);

        if (canUseCache) {
            for (auto &savedNext: forwardCache) {
                cache->registerForwardMapping(stmtNum, savedNext);
            }
            return false;
        }
        return true;
    };

    TerminateCheck canEnd = [](CFGNode *) { return false; };

    runBFS(true, saveToCache, canEnd, node);
}

void AdaptersUtils::runUpBFS(const string &stmtNum, Cache *cache, CFGNode *node) {
    CacheCallback saveToCache = [stmtNum, &cache](const string &next) {
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

    TerminateCheck canEnd = [](CFGNode *) { return false; };

    runBFS(false, saveToCache, canEnd, node);
}

void AdaptersUtils::fullBFS(Cache *cache, CFGNode *node) {
    unordered_set<string> mainVisited;
    queue<CFGNode *> mainQ;

    // we cannot start with root its 0!
    for (auto &startProc: node->getChildren()) {
        mainQ.push(startProc);
        mainVisited.insert(startProc->getStmtNum());
    }

    while (!mainQ.empty()) {
        CFGNode *curr = mainQ.front();
        vector<CFGNode *> children = curr->getChildren();
        string currStmtNum = curr->getStmtNum();

        CacheCallback saveToCache = [currStmtNum, &cache](const string &next) {
            cache->addAllMappingPair({currStmtNum, next});
            // Add forward Mapping
            cache->registerForwardMapping(currStmtNum, next);
            // Add backwardMapping
            cache->registerBackwardMapping(next, currStmtNum);

            return true;
        };

        TerminateCheck canEnd = [&mainVisited, &mainQ](CFGNode *next) {
            string nextStmtNum = next->getStmtNum();
            if (mainVisited.find(nextStmtNum) == mainVisited.end()) {
                mainQ.push(next);
                mainVisited.insert(nextStmtNum);
            }
            return false;
        };

        runBFS(true, saveToCache, canEnd, curr);
        mainQ.pop();
    }
}

CFGNode *AdaptersUtils::getStartingParentNode(CFGNode *rootCFG, const string &stmt) {
    int stmtNum = stoi(stmt);

    vector<CFGNode *> children = rootCFG->getChildren();

    // Binary search for first element that is greater than stmt
    auto result = std::upper_bound(children.begin(), children.end(), stmtNum, [](int value, CFGNode *currNode) {
        return value < stoi(currNode->getStmtNum());
    });

    if (result == children.begin()) {
        return nullptr;
    }

    // Get the first stmt of procedure
    // Note: does not check if stmt is out of range
    return *(--result);
}
