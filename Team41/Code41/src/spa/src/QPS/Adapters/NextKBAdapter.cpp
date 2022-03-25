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
                    addFullMapping(currStmtNum, nextStmtNum);
                    bfsQueue.push(next);
                }
            }
        }

        bfsVisited.clear();
    }
}

void NextKBAdapter::addFullMapping(const string &start, const string &end) {
    cache->registerBooleanMapping(start, end);
    // Add forward Mapping
    cache->registerForwardMapping(start, end);
    // Add backwardMapping
    cache->registerBackwardMapping(end, start);
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
    if (!backwardCache.empty())
        return backwardCache.find(stmt1) != backwardCache.end();

    unordered_set<string> forwardCache = cache->getForwardMapping(stmt1);
    if (!forwardCache.empty())
        return forwardCache.find(stmt2) != forwardCache.end();

    if (!cache->getBooleanMapping(stmt1, stmt2)) {
        CFGNode *startNode = pkb->getCFGForStmt(stmt1);
        AdaptersUtils::runBoolBFS(stmt1, stmt2, cache, startNode);
    }


    return cache->getBooleanMapping(stmt1, stmt2);
}

unordered_set<string> NextKBAdapter::getAllStmtsNextT(const string &stmtNum) {
    if (cache->getForwardMapping(stmtNum).empty()) {
        CFGNode *startNode = pkb->getCFGForStmt(stmtNum);
        AdaptersUtils::runDownBFS(stmtNum, cache, startNode);
    }

    return cache->getForwardMapping(stmtNum);
}

unordered_set<string> NextKBAdapter::getAllStmtsTBefore(const string &stmtNum) {
    if (cache->getBackwardMapping(stmtNum).empty()) {
        CFGNode *endNode = pkb->getCFGForStmt(stmtNum);
        AdaptersUtils::runUpBFS(stmtNum, cache, endNode);
    }

    return cache->getBackwardMapping(stmtNum);
}

vector<pair<string, string>> NextKBAdapter::getAllNextT() {
    if (cache->getAllMapping().empty())
        fullBFS();

    return cache->getAllMapping();
}

vector<string> NextKBAdapter::getAllStmtsThatHaveNextTStmt() {
    return pkb->getAllStmtsExecBeforeSomeStmt();
}

vector<string> NextKBAdapter::getAllStmtsThatIsNextTOfSomeStmt() {
    return pkb->getAllStmtsExecAfterSomeStmt();
}

NextKBAdapter::~NextKBAdapter() {
    delete cache;
}
