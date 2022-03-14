#include "NextKBAdapter.h"

NextKBAdapter::NextKBAdapter(PKBClient* pkb) : pkb(pkb) {
    this->cache = new Cache();
}

void NextKBAdapter::booleanBFS(string &start, string &end) {
    CFGNode* startNode = pkb->getCFGForStmt(start);

    if (startNode == nullptr) return;

    queue<CFGNode *> bfsQueue;
    string currStmtNum = startNode->getStmtNum();

    for (auto& next : startNode->getChildren()) {
        string nextStmtNum = next->getStmtNum();
        bool hasMapping = cache->getBooleanMapping(currStmtNum, nextStmtNum);

        if (hasMapping) continue;

        addBooleanRelation(currStmtNum, nextStmtNum);
        bfsQueue.push(next);

        if (nextStmtNum == end) return;
    }

    unordered_set<string> visited;

    while (!bfsQueue.empty()) {
        CFGNode* currNode = bfsQueue.front();
        bfsQueue.pop();
        // TODO: why Local variable 'currNode' may point to invalidated memory ?
        visited.insert(currNode->getStmtNum());

        for (auto& next : currNode->getChildren()) {
            string nextStmtNum = next->getStmtNum();
            if (visited.find(nextStmtNum) == visited.end()) {
                bool hasMapping = cache->getBooleanMapping(currStmtNum, nextStmtNum);

                if (hasMapping) continue;

                addBooleanRelation(currStmtNum, nextStmtNum);
                bfsQueue.push(next);

                if (nextStmtNum == end) return;
            }
        }
    }
}

void NextKBAdapter::forwardBFS(string &start) {
    CFGNode* startNode = pkb->getCFGForStmt(start);

    if (startNode == nullptr) return;

    queue<CFGNode *> bfsQueue;
    string currStmtNum = startNode->getStmtNum();

    for (auto& next : startNode->getChildren()) {
        string nextStmtNum = next->getStmtNum();
        unordered_set<string> mapping = cache->getForwardMapping(nextStmtNum);

        if (!mapping.empty()) {
            addForwardMapping(currStmtNum, mapping);
            continue;
        }

        addForwardRelation(currStmtNum, nextStmtNum);
        bfsQueue.push(next);
    }

    unordered_set<string> visited;

    while (!bfsQueue.empty()) {
        CFGNode* currNode = bfsQueue.front();
        bfsQueue.pop();
        visited.insert(currNode->getStmtNum());

        for (auto& next : currNode->getChildren()) {
            string nextStmtNum = next->getStmtNum();
            if (visited.find(nextStmtNum) == visited.end()) {
                unordered_set<string> mapping = cache->getForwardMapping(nextStmtNum);

                if (!mapping.empty()) {
                    addForwardMapping(currStmtNum, mapping);
                    continue;
                }

                addForwardRelation(currStmtNum, nextStmtNum);
                bfsQueue.push(next);
            }
        }
    }
}

void NextKBAdapter::backwardBFS(string &start) {
    CFGNode* startNode = pkb->getCFGForStmt(start);

    if (startNode == nullptr) return;

    queue<CFGNode *> bfsQueue;
    string currStmtNum = startNode->getStmtNum();

    for (auto& next : startNode->getParent()) {
        string nextStmtNum = next->getStmtNum();
        unordered_set<string> mapping = cache->getBackwardMapping(nextStmtNum);

        if (!mapping.empty()) {
            addBackwardMapping(currStmtNum, mapping);
            continue;
        }

        addBackwardRelation(currStmtNum, nextStmtNum);
        bfsQueue.push(next);
    }

    unordered_set<string> visited;

    while (!bfsQueue.empty()) {
        CFGNode* currNode = bfsQueue.front();
        bfsQueue.pop();
        visited.insert(currNode->getStmtNum());

        for (auto& next : currNode->getParent()) {
            string nextStmtNum = next->getStmtNum();
            if (visited.find(nextStmtNum) == visited.end()) {
                unordered_set<string> mapping = cache->getBackwardMapping(nextStmtNum);

                if (!mapping.empty()) {
                    addBackwardMapping(currStmtNum, mapping);
                    continue;
                }

                addBackwardRelation(currStmtNum, nextStmtNum);
                bfsQueue.push(next);
            }
        }
    }
}

void NextKBAdapter::fullBFS() {
    CFGNode *start = pkb->getRootCFG();

    unordered_set<string> mainVisited;
    queue<CFGNode *> mainQ;
    mainQ.push(start);

    unordered_set<string> bfsVisited;
    queue<CFGNode *> bfsQueue;

    while (!mainQ.empty()) {
        CFGNode *curr = mainQ.front();
        mainQ.pop();

        string currStmtNum = curr->getStmtNum();

        for (auto& next : curr->getChildren()) {
            string nextStmtNum = next->getStmtNum();
            cache->addAllMappingPair({currStmtNum, nextStmtNum});
            bfsQueue.push(next);

            if (mainVisited.find(nextStmtNum) == mainVisited.end()) {
                mainQ.push(next);
                mainVisited.insert(nextStmtNum);
            }
        }

        while (!bfsQueue.empty()) {
            CFGNode* currNode = bfsQueue.front();
            bfsQueue.pop();
            bfsVisited.insert(currNode->getStmtNum());

            for (auto& next : currNode->getParent()) {
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

// TODO: change when figure out how to use function pointer
void NextKBAdapter::addForwardMapping(const string &currStmtNum, unordered_set<string> &mapping) {
    for (auto& savedNext : mapping)
        addForwardRelation(currStmtNum, savedNext);
}

// TODO: change when figure out how to use function pointer
void NextKBAdapter::addBackwardMapping(const string &currStmtNum, unordered_set<string> &mapping) {
    for (auto& savedNext : mapping)
        addBackwardRelation(currStmtNum, savedNext);
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
    if (!cache->getBooleanMapping(stmt1, stmt2)) {
        booleanBFS(stmt1, stmt2);
    }

    return cache->getBooleanMapping(stmt1, stmt2);
}

unordered_set<string> NextKBAdapter::getAllStmtsNextT(string stmtNum) {
    if (cache->getForwardMapping(stmtNum).empty()) {
        forwardBFS(stmtNum);
    }

    return cache->getForwardMapping(stmtNum);
}

unordered_set<string> NextKBAdapter::getAllStmtsTBefore(string stmtNum) {
    if (cache->getBackwardMapping(stmtNum).empty()) {
        backwardBFS(stmtNum);
    }

    return cache->getBackwardMapping(stmtNum);
}

vector<pair<string, string>> NextKBAdapter::getAllNextT() {
    if (cache->getAllMapping().empty()) {
        // populate
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
