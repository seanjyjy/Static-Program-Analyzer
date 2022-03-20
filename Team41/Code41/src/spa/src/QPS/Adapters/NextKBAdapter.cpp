#include "NextKBAdapter.h"

NextKBAdapter::NextKBAdapter(PKBClient *pkb) : pkb(pkb) {
    this->cache = new Cache();
}

//================================== Private =============================================

void NextKBAdapter::fullBFS() {
    CFGNode *start = pkb->getRootCFG();

    unordered_set<string> mainVisited;
    queue<CFGNode *> mainQ;

    // we cannot start with root its 0!
    for (auto &startProc: start->getChildren()) {
        mainQ.push(startProc);
        mainVisited.insert(startProc->getStmtNum());
    }

    unordered_set<string> bfsVisited;
    queue<CFGNode *> bfsQueue;

    while (!mainQ.empty()) {
        CFGNode *curr = mainQ.front();
        vector<CFGNode *> children = curr->getChildren();
        string currStmtNum = curr->getStmtNum();
        mainQ.pop();

        for (auto &next: children) {
            string nextStmtNum = next->getStmtNum();
            cache->addAllMappingPair({currStmtNum, nextStmtNum});
            addFullMapping(currStmtNum, nextStmtNum);
            bfsQueue.push(next);

            if (mainVisited.find(nextStmtNum) == mainVisited.end()) {
                mainQ.push(next);
                mainVisited.insert(nextStmtNum);
            }
        }

        while (!bfsQueue.empty()) {
            CFGNode *currNode = bfsQueue.front();
            bfsVisited.insert(currNode->getStmtNum());
            vector<CFGNode *> currChildren = currNode->getChildren();
            bfsQueue.pop();

            for (auto &next: currChildren) {
                string nextStmtNum = next->getStmtNum();

                if (bfsVisited.find(nextStmtNum) == bfsVisited.end()) {
                    cache->addAllMappingPair({currStmtNum, nextStmtNum});
                    bfsQueue.push(next);
                }
            }
        }

        bfsVisited.clear();
    }
}

void
NextKBAdapter::runBFS(const string &start, const string &end, bool isForward, const CacheCallback &cacheAndContinue,
                      const TerminateCheck &canTerminate) {
    CFGNode *startNode = pkb->getCFGForStmt(isForward ? start : end);

    if (startNode == nullptr) return;

    queue<CFGNode *> bfsQueue;
    unordered_set<string> visited;
    bfsQueue.push(startNode);

    while (!bfsQueue.empty()) {
        CFGNode *currNode = bfsQueue.front();
        vector<CFGNode *> nextNodes = isForward ? currNode->getChildren() : currNode->getParent();
        bfsQueue.pop();

        for (auto &next: nextNodes) {
            string nextStmtNum = next->getStmtNum();
            if (visited.find(nextStmtNum) == visited.end()) {
                // check cache
                if (cacheAndContinue(nextStmtNum)) {
                    bfsQueue.push(next);
                }
                if (canTerminate(nextStmtNum)) {
                    return;
                };
                visited.insert(nextStmtNum);
            }
        }
    }
}

void NextKBAdapter::addBooleanRelation(const string &start, const string &end) {
    cache->registerBooleanMapping(start, end);
}

void NextKBAdapter::addForwardRelation(const string &start, const string &end) {
    cache->registerBooleanMapping(start, end);
    cache->registerForwardMapping(start, end);
}

void NextKBAdapter::addBackwardRelation(const string &start, const string &end) {
    cache->registerBooleanMapping(end, start); // reverse order!
    cache->registerBackwardMapping(start, end);
}

void NextKBAdapter::addFullMapping(const string &start, const string &end) {
    addForwardRelation(start, end);
    addBackwardRelation(start, end);
}

//================================== Public =============================================

bool NextKBAdapter::isNext(string stmt1, string stmt2) const {
    return pkb->isNext(move(stmt1), move(stmt2));
}

vector<CFGNode *> NextKBAdapter::getNextNodes(string stmtNum) const {
    return pkb->getNextNodes(move(stmtNum));
}

vector<CFGNode *> NextKBAdapter::getPrevNodes(string stmtNum) const {
    return pkb->getPrevNodes(move(stmtNum));
}

vector<pair<string, string>> NextKBAdapter::getAllNext() const {
    return pkb->getAllNext();
}

vector<string> NextKBAdapter::getAllStmtsThatHaveNextStmt() const {
    return pkb->getAllStmtsExecBeforeSomeStmt();
}

vector<string> NextKBAdapter::getAllStmtsThatIsNextOfSomeStmt() const {
    return pkb->getAllStmtsExecAfterSomeStmt();
}

bool NextKBAdapter::isNextT(const string &stmt1, const string &stmt2) {
    unordered_set<string> backwardCache = cache->getBackwardMapping(stmt2);
    if (!backwardCache.empty()) {
        return backwardCache.find(stmt1) != backwardCache.end();
    }

    unordered_set<string> forwardCache = cache->getForwardMapping(stmt1);
    if (!forwardCache.empty()) {
        return forwardCache.find(stmt2) != forwardCache.end();
    }

    if (!cache->getBooleanMapping(stmt1, stmt2)) {
        CacheCallback saveToCache = [&](const string &next) {
            addBooleanRelation(stmt1, next);
            return true;
        };

        TerminateCheck canEnd = [&](const string &next) {
            if (cache->getBooleanMapping(next, stmt2)) {
                addBooleanRelation(stmt1, stmt2);
                return true;
            };
            unordered_set<string> forwardCache = cache->getForwardMapping(next);
            if (!forwardCache.empty()) {
                bool hasNextT = forwardCache.find(stmt2) != forwardCache.end();
                if (hasNextT) {
                    addBooleanRelation(stmt1, stmt2);
                }
                return hasNextT;
            }
            return false;
        };
        runBFS(stmt1, stmt2, true, saveToCache, canEnd);
    }

    return cache->getBooleanMapping(stmt1, stmt2);
}

unordered_set<string> NextKBAdapter::getAllStmtsNextT(const string &stmtNum) {
    if (cache->getForwardMapping(stmtNum).empty()) {
        CacheCallback saveToCache = [&](const string &next) {
            unordered_set<string> forwardCache = cache->getForwardMapping(next);
            bool canUseCache = !forwardCache.empty() && stmtNum != next;

            if (canUseCache) {
                for (auto &savedNext: forwardCache)
                    addForwardRelation(stmtNum, savedNext);
                return false;
            }
            addForwardRelation(stmtNum, next);
            return true;
        };

        TerminateCheck canEnd = [](const string &next) { return false; };

        runBFS(stmtNum, ROOT_STMT, true, saveToCache, canEnd);
    }

    return cache->getForwardMapping(stmtNum);
}

unordered_set<string> NextKBAdapter::getAllStmtsTBefore(const string &stmtNum) {
    if (cache->getBackwardMapping(stmtNum).empty()) {
        CacheCallback saveToCache = [&](const string &next) {
            unordered_set<string> backwardCache = cache->getBackwardMapping(next);
            bool canUseCache = !backwardCache.empty() && stmtNum != next;

            if (canUseCache) {
                for (auto &savedNext: backwardCache)
                    addBackwardRelation(stmtNum, savedNext);
                return false;
            }
            addBackwardRelation(stmtNum, next);
            return true;
        };

        TerminateCheck canEnd = [](const string &next) { return false; };

        runBFS(ROOT_STMT, stmtNum, false, saveToCache, canEnd);
    }

    return cache->getBackwardMapping(stmtNum);
}

vector<pair<string, string>> NextKBAdapter::getAllNextT() {
    if (cache->getAllMapping().empty()) {
        fullBFS();
    }

    return cache->getAllMapping();
}

vector<string> NextKBAdapter::getAllStmtsThatHaveNextTStmt() {
    return pkb->getAllStmtsExecBeforeSomeStmt();
}

vector<string> NextKBAdapter::getAllStmtsThatIsNextTOfSomeStmt() {
    return pkb->getAllStmtsExecAfterSomeStmt();
}
