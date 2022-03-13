#include "NextKBAdapter.h"

NextKBAdapter::NextKBAdapter(PKBClient* pkb) : pkb(pkb) {
    this->cache = new Cache();
}

vector<string> NextKBAdapter::dfsWrapper(string &start, string &end) {
    unordered_set<string> visited;
    vector<string> path;
    // Assume we have a map
    CFGNode* startNode = map.get(start);

    dfs(startNode, path, visited, end);

    return path;
}

// Need not be shortest path just any path
void NextKBAdapter::dfs(CFGNode *node, vector<string> &path, unordered_set<string> &visited, string &end) {
    if (node->stmtNum == end) {
        return;
    }

    path.insert(node->stmtNum);
    visited.insert(node->stmtNum);

    for (auto& child : node->getChildren()) {
        if (visited.find(child->stmtNum) == visited.end()) {
            addRelation(node->stmtNum, child->stmtNum);
            dfs(child, path, visited, end);
        }
    }

    path.pop_back();
}

void NextKBAdapter::addRelation(const string& start, const string& end) {
    cache->registerBooleanMapping(start, end);
    cache->registerForwardMapping(start, end);
    cache->registerBackwardMapping(end, start);
}

bool NextKBAdapter::isNext(string stmt1, string stmt2) const {
    return false;
}

unordered_set<string> NextKBAdapter::getAllStmtsNext(string stmtNum) const {
    return unordered_set<string>();
}

unordered_set<string> NextKBAdapter::getAllStmtsBefore(string stmtNum) const {
    return unordered_set<string>();
}

vector<pair<string, string>> NextKBAdapter::getAllNext() const {
    return vector<pair<string, string>>();
}

unordered_set<string> NextKBAdapter::getAllStmtsThatHaveNextStmt() const {
    return unordered_set<string>();
}

unordered_set<string> NextKBAdapter::getAllStmtsThatIsNextOfSomeStmt() const {
    return unordered_set<string>();
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
