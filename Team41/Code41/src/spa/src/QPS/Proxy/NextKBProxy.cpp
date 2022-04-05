#include "NextKBProxy.h"

NextKBProxy::NextKBProxy(PKBClient *pkb) : pkb(pkb) {
    this->cache = new Cache();
}

//================================== Public =============================================

bool NextKBProxy::isNext(string stmt1, string stmt2) const {
    return pkb->isNext(move(stmt1), move(stmt2));
}

vector<CFGNode *> NextKBProxy::getNextNodes(string stmtNum) const {
    return pkb->getNextNodes(move(stmtNum));
}

vector<CFGNode *> NextKBProxy::getPrevNodes(string stmtNum) const {
    return pkb->getPrevNodes(move(stmtNum));
}

vector<pair<string, string>> NextKBProxy::getAllNext() const {
    return pkb->getAllNext();
}

vector<string> NextKBProxy::getAllStmtsThatHaveNextStmt() const {
    return pkb->getAllStmtsExecBeforeSomeStmt();
}

vector<string> NextKBProxy::getAllStmtsThatIsNextOfSomeStmt() const {
    return pkb->getAllStmtsExecAfterSomeStmt();
}

bool NextKBProxy::isNextT(const string &stmt1, const string &stmt2) {
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
    CFGNode *firstNode = ProxyUtils::getStartingParentNode(rootCFG, stmt1);
    CFGNode *secondNode = ProxyUtils::getStartingParentNode(rootCFG, stmt2);
    if (firstNode == nullptr || firstNode != secondNode) {
        return false;
    }

    // check if boolean cache exist
    if (!cache->getBooleanMapping(stmt1, stmt2)) {
        CFGNode *startNode = pkb->getCFGForStmt(stmt1);
        ProxyUtils::runBoolBFS(stmt1, stmt2, cache, startNode);
    }

    return cache->getBooleanMapping(stmt1, stmt2);
}

unordered_set<string> NextKBProxy::getAllStmtsNextT(const string &stmtNum) {
    if (cache->getForwardMapping(stmtNum).empty()) {
        CFGNode *startNode = pkb->getCFGForStmt(stmtNum);
        ProxyUtils::runDownBFS(stmtNum, cache, startNode);
    }

    return cache->getForwardMapping(stmtNum);
}

unordered_set<string> NextKBProxy::getAllStmtsTBefore(const string &stmtNum) {
    if (cache->getBackwardMapping(stmtNum).empty()) {
        CFGNode *endNode = pkb->getCFGForStmt(stmtNum);
        ProxyUtils::runUpBFS(stmtNum, cache, endNode);
    }

    return cache->getBackwardMapping(stmtNum);
}

vector<pair<string, string>> NextKBProxy::getAllNextT() {
    if (cache->getAllMapping().empty()) {
        CFGNode *start = pkb->getRootCFG();
        ProxyUtils::fullBFS(cache, start);
    }
    return cache->getAllMapping();
}

vector<string> NextKBProxy::getAllStmtsThatHaveNextTStmt() const {
    return pkb->getAllStmtsExecBeforeSomeStmt();
}

vector<string> NextKBProxy::getAllStmtsThatIsNextTOfSomeStmt() const {
    return pkb->getAllStmtsExecAfterSomeStmt();
}

NextKBProxy::~NextKBProxy() {
    delete cache;
}
