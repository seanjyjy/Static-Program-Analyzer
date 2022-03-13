#include "NextKBAdapter.h"

NextKBAdapter::NextKBAdapter(PKBClient* pkb) : pkb(pkb) {
    this->cache = new Cache();
}

void NextKBAdapter::booleanBFSWrapper(string &start, string &end) {
    CFGNode* startNode = pkb->getCFGForStmt(start);

    if (startNode == nullptr) {
        return;
    }

    queue<CFGNode *> bfsQueue;
    unordered_set<string> visited;

    string currStmtNum = startNode->getStmtNum();

    for (auto& next : startNode->getChildren()) {
        string nextStmtNum = next->getStmtNum();
        addBooleanRelation(currStmtNum, nextStmtNum);
        bfsQueue.push(next);

        if (nextStmtNum == end) {
            return;
        }
    }

    booleanBFS(visited, end, currStmtNum, bfsQueue);
}

// Need not explore all possible path. As long as reach the end, we will terminate
// Worst case scenario this will perform like forwardDFS
void NextKBAdapter::booleanBFS(unordered_set<string> &visited, string &end, string& currStmtNum,
                               queue<CFGNode *> &bfsQueue) {

    while (!bfsQueue.empty()) {
        CFGNode* currNode = bfsQueue.front();
        bfsQueue.pop();
        visited.insert(currNode->getStmtNum());

        for (auto& next : currNode->getChildren()) {
            if (visited.find(next->getStmtNum()) == visited.end()) {
                string nextStmtNum = next->getStmtNum();
                addBooleanRelation(currStmtNum, nextStmtNum);
                bfsQueue.push(next);

                if (nextStmtNum == end) {
                    return;
                }
            }
        }
    }
}

void NextKBAdapter::forwardDFSWrapper(string &start) {
    unordered_set<string> visited;
    CFGNode* startNode = pkb->getCFGForStmt(start);

    if (startNode == nullptr) {
        return;
    }

    forwardDFS(startNode, visited);
}

void NextKBAdapter::forwardDFS(CFGNode *node, unordered_set<string> &visited) {

}

void NextKBAdapter::backwardDFSWrapper(string &end) {
    unordered_set<string> visited;
    CFGNode* startNode = pkb->getCFGForStmt(end);

    if (startNode == nullptr) {
        return;
    }

    backwardDFS(startNode, visited);
}

void NextKBAdapter::backwardDFS(CFGNode *node, unordered_set<string> &visited) {

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
    bool res = cache->getBooleanMapping(stmt1, stmt2);
    if (!res) {
        booleanBFSWrapper(stmt1, stmt2);
    }

    return res;
}

// Let 9 be the largest stmtNum, it NextT(5, s) is queried,
// NextT(6, s), NextT(7, s), NextT(8, s), NextT(9, s) result will be cache
unordered_set<string> NextKBAdapter::getAllStmtsNextT(string stmtNum) {
    unordered_set<string> mapping = cache->getForwardMapping(stmtNum);
    if (mapping.empty()) {
        forwardDFSWrapper(stmtNum);
    }

    return mapping;
}

unordered_set<string> NextKBAdapter::getAllStmtsTBefore(string stmtNum) {
    unordered_set<string> mapping = cache->getBackwardMapping(stmtNum);
    if (mapping.empty()) {
        backwardDFSWrapper(stmtNum);
    }

    return mapping;
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