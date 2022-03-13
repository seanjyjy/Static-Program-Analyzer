#include "NextKBAdapter.h"

NextKBAdapter::NextKBAdapter(PKBClient* pkb) : pkb(pkb) {
    this->cache = new Cache();
}

vector<string> NextKBAdapter::booleanDFSWrapper(string &start, string &end) {
    unordered_set<string> visited;
    vector<string> path;
    // Assume we have a map
    CFGNode* startNode = pkb->getCFGForStmt(start);

    if (startNode == nullptr) {
        return path;
    }

    booleanDFS(startNode, path, visited, end);

    return path;
}

// Need not be shortest path just any path
void NextKBAdapter::booleanDFS(CFGNode *node, vector<string> &path, unordered_set<string> &visited, string &end) {
    if (node->getStmtNum() == end) {
        return;
    }

    path.push_back(node->getStmtNum());
    visited.insert(node->getStmtNum());

    for (auto& child : node->getChildren()) {
        if (visited.find(child->getStmtNum()) == visited.end()) {
            addBooleanRelation(node->getStmtNum(), child->getStmtNum());
            booleanDFS(child, path, visited, end);
        }
    }

    path.pop_back();
}

void NextKBAdapter::addBooleanRelation(const string& start, const string& end) {
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

bool NextKBAdapter::isNextT(string stmt1, string stmt2) {
    if (cache->getBooleanMapping(stmt1, stmt2)) {
        return true;
    }

    vector<string> path = booleanDFSWrapper(stmt1, stmt2);

    // checks if start is stmt1 and end is stmt2
    return path.at(0) == stmt1 && path.at(path.size() - 1) == stmt2;
}

// Let 9 be the largest stmtNum, it NextT(5, s) is queried,
// NextT(6, s), NextT(7, s), NextT(8, s), NextT(9, s) result will be cache
unordered_set<string> NextKBAdapter::getAllStmtsNextT(string stmtNum) {
    if (!cache->getForwardMapping(stmtNum).empty()) {
        return cache->getForwardMapping(stmtNum);
    }


}

unordered_set<string> NextKBAdapter::getAllStmtsTBefore(string stmtNum) {
    if (!cache->getBackwardMapping(stmtNum).empty()) {
        return cache->getBackwardMapping(stmtNum);
    }


}

vector<pair<string, string>> NextKBAdapter::getAllNextT() {
    // Check forward first stmt and backward last stmt
    return vector<pair<string, string>>();
}

unordered_set<string> NextKBAdapter::getAllStmtsThatHaveNextTStmt() {
    return unordered_set<string>();
}

unordered_set<string> NextKBAdapter::getAllStmtThatIsNextTOfSomeStmt() {
    return unordered_set<string>();
}