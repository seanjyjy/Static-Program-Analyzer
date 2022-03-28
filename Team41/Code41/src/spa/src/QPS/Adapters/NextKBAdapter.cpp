#include "NextKBAdapter.h"

NextKBAdapter::NextKBAdapter(PKBClient *pkb) : pkb(pkb) {
    this->cache = new Cache();
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
    // check if backward cache exist
    unordered_set<string> backwardCache = cache->getBackwardMapping(stmt2);
    if (!backwardCache.empty())
        return backwardCache.find(stmt1) != backwardCache.end();

    // check if forward cache exist
    unordered_set<string> forwardCache = cache->getForwardMapping(stmt1);
    if (!forwardCache.empty())
        return forwardCache.find(stmt2) != forwardCache.end();

    // check if same procedure
    CFGNode *rootCFG = pkb->getRootCFG();
    CFGNode *firstNode = AdaptersUtils::getStartingParentNode(rootCFG, stmt1);
    CFGNode *secondNode = AdaptersUtils::getStartingParentNode(rootCFG, stmt2);
    if (firstNode == nullptr || firstNode != secondNode) {
        return false;
    }

    // check if boolean cache exist
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
    if (cache->getAllMapping().empty()) {
        CFGNode *start = pkb->getRootCFG();
        AdaptersUtils::fullBFS(cache, start);
    }
    return cache->getAllMapping();
}

vector<string> NextKBAdapter::getAllStmtsThatHaveNextTStmt() const {
    return pkb->getAllStmtsExecBeforeSomeStmt();
}

vector<string> NextKBAdapter::getAllStmtsThatIsNextTOfSomeStmt() const {
    return pkb->getAllStmtsExecAfterSomeStmt();
}

NextKBAdapter::~NextKBAdapter() {
    delete cache;
}
