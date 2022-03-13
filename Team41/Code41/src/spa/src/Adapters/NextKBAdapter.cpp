#include "NextKBAdapter.h"

NextKBAdapter::NextKBAdapter(PKBClient* pkb) : pkb(pkb) {
    this->cache = new Cache();
}

vector<string> NextKBAdapter::dfsWrapper(string &start, string &end) {
    unordered_set<string> visited;
    vector<string> path;
    // Assume we have a map
    CFGNode* startNode = this->pkb->getCFGForStmt(start);

    dfs(startNode, path, visited, end);

    return path;
}

// Need not be shortest path just any path
void NextKBAdapter::dfs(CFGNode *node, vector<string> &path, unordered_set<string> &visited, string &end) {
    if (node->getStmtNum() == end) {
        return;
    }

    path.insert(node->getStmtNum());
    visited.insert(node->getStmtNum());

    for (auto& child : node->getChildren()) {
        if (visited.find(child->getStmtNum()) == visited.end()) {
            addBooleanRelation(node->getStmtNum(), child->getStmtNum());
            dfs(child, path, visited, end);
        }
    }

    path.pop_back();
}

void NextKBAdapter::addBooleanRelation(const string& start, const string& end) {
    cache->registerBooleanMapping(start, end);
}

void NextKBAdapter::addForwardRelation(const string &start, const string &end) {

}

void NextKBAdapter::addBackwardRelation(const string &start, const string &end) {

}

bool NextKBAdapter::isNext(string stmt1, string stmt2) const {
    return pkb->isNext(move(stmt1), move(stmt2));
}

unordered_set<string> NextKBAdapter::getAllStmtsNext(string stmtNum) const {
    return pkb->getNextStmts(move(stmtNum));
}

unordered_set<string> NextKBAdapter::getAllStmtsBefore(string stmtNum) const {
    return pkb->getPrevStmts(move(stmtNum));
}

vector<pair<string, string>> NextKBAdapter::getAllNext() const {
    return pkb->getAllNext();
}

unordered_set<string> NextKBAdapter::getAllStmtsThatHaveNextStmt() const {
    return pkb->getAllStmtsExecBeforeSomeStmt();
}

unordered_set<string> NextKBAdapter::getAllStmtsThatIsNextOfSomeStmt() const {
    return pkb->getAllStmtsExecAfterSomeStmt();
}

bool NextKBAdapter::isNextT(string stmt1, string stmt2) const {
    if (this->cache->getBooleanMapping(stmt1, stmt2)) {
        return true;
    }

    // checks if start is stmt1 and end is stmt2
}

unordered_set<string> NextKBAdapter::getAllStmtsNextT(string stmtNum) const {
    return unordered_set<string>();
}

unordered_set<string> NextKBAdapter::getAllStmtsTBefore(string stmtNum) const {
    return unordered_set<string>();
}

vector<pair<string, string>> NextKBAdapter::getAllNextT() const {
    return vector<pair<string, string>>();
}

unordered_set<string> NextKBAdapter::getAllStmtsThatHaveNextTStmt() const {
    return unordered_set<string>();
}

unordered_set<string> NextKBAdapter::getAllStmtThatIsNextTOfSomeStmt() const {
    return unordered_set<string>();
}