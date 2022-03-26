#include "AffectsKBAdapter.h"

AffectsKBAdapter::AffectsKBAdapter(PKBClient *pkb) : pkb(pkb) {
    this->cache = new Cache();
    stmtNumToNodeMap.insert({ROOT_CFG,  new CFGNode(ROOT_CFG)});
}

AffectsKBAdapter::~AffectsKBAdapter() { delete cache; }

bool AffectsKBAdapter::isModifyStmt(const string &stmtNum) {
    return pkb->isAssignStmt(stmtNum) || pkb->isCallStmt(stmtNum) || pkb->isReadStmt(stmtNum);
}

bool AffectsKBAdapter::isAffects(const string &stmtNum1, const string &stmtNum2) {
    if (hasAffectsGraph()) {
        if (stmtNumToNodeMap.find(stmtNum1) == stmtNumToNodeMap.end())
            return false;

        CFGNode *node = stmtNumToNodeMap.at(stmtNum1);
        for (auto child: node->getChildren()) {
            if (child->getStmtNum() == stmtNum2)
                return true;
        }
        return false;
    }

    CFGNode *start = pkb->getCFGForStmt(stmtNum1);
    string variable = *pkb->getModifiesByStmt(stmtNum1).begin();

    unordered_set<string> variablesUsed = pkb->getUsesByStmt(stmtNum2);
    // the variable that was modified in stmt1 isnt being used in stmt2
    if (variablesUsed.find(variable) == variablesUsed.end())
        return false;

    bool isAffects = bfsBool(start, variable, stmtNum2);
    return isAffects;
}

unordered_set<string> AffectsKBAdapter::getDirectAffectsBy(const string &stmtNum) {
    unordered_set<string> affected;

    if (hasAffectsGraph()) {
        if (stmtNumToNodeMap.find(stmtNum) == stmtNumToNodeMap.end())
            return {};

        CFGNode *node = stmtNumToNodeMap.at(stmtNum);
        for (auto child: node->getChildren())
            affected.insert(child->getStmtNum());
        return affected;
    }

    CFGNode *start = pkb->getCFGForStmt(stmtNum);
    // we are ensured that there is a modified var since this is an assignment statement
    string variable = *pkb->getModifiesByStmt(stmtNum).begin();

    bfsDown(start, variable, affected);
    return affected;
}

unordered_set<string> AffectsKBAdapter::getDirectAffecting(const string &stmtNum) {
    unordered_set<string> affecting;

    if (hasAffectsGraph()) {
        if (stmtNumToNodeMap.find(stmtNum) == stmtNumToNodeMap.end())
            return {};

        CFGNode *node = stmtNumToNodeMap.at(stmtNum);
        for (auto parent: node->getParent())
            affecting.insert(parent->getStmtNum());
        return affecting;
    }

    CFGNode *start = pkb->getCFGForStmt(stmtNum);
    unordered_set<string> affectedVars = pkb->getUsesByStmt(stmtNum);

    bfsUp(start, affectedVars, affecting);
    return affecting;
}

unordered_set<string> AffectsKBAdapter::getAllStmtAffectingOther() {
    if (!hasAffectsGraph())
        buildAffectsGraph();

    return affectings;
}

unordered_set<string> AffectsKBAdapter::getAllStmtAffectedByOther() {
    if (!hasAffectsGraph())
        buildAffectsGraph();

    return affecteds;
}

vector<pair<string, string>> AffectsKBAdapter::getDirectAffectsAll() {
    if (!hasAffectsGraph())
        buildAffectsGraph();

    return affectingAffectedPairs;
}

bool AffectsKBAdapter::isAffectsT(const string &stmtNum1, const string &stmtNum2) {
    if (!hasAffectsGraph(stmtNum1) && !hasAffectsGraph(stmtNum2)) {
        CFGNode* root = pkb->getRootCFG();
        CFGNode *firstNode = AdaptersUtils::getStartingParentNode(root, stmtNum1);
        CFGNode *secondNode = AdaptersUtils::getStartingParentNode(root, stmtNum2);
        if (firstNode == nullptr || firstNode != secondNode) {
            return false;
        }
        buildAffectsGraphForProc(firstNode);
    }

    unordered_set<string> backwardCache = cache->getBackwardMapping(stmtNum2);
    if (!backwardCache.empty())
        return backwardCache.find(stmtNum1) != backwardCache.end();

    unordered_set<string> forwardCache = cache->getForwardMapping(stmtNum1);
    if (!forwardCache.empty())
        return forwardCache.find(stmtNum2) != forwardCache.end();

    if (!cache->getBooleanMapping(stmtNum1, stmtNum2)) {
        if (stmtNumToNodeMap.find(stmtNum1) == stmtNumToNodeMap.end())
            return false;

        CFGNode *startNode = stmtNumToNodeMap.at(stmtNum1);
        AdaptersUtils::runBoolBFS(stmtNum1, stmtNum2, cache, startNode);
    }

    return cache->getBooleanMapping(stmtNum1, stmtNum2);
}

unordered_set<string> AffectsKBAdapter::getAffectsTBy(const string &stmtNum) {
    if (!hasAffectsGraph(stmtNum)) {
        CFGNode *firstNode = AdaptersUtils::getStartingParentNode(pkb->getRootCFG(), stmtNum);
        buildAffectsGraphForProc(firstNode);
    }

    if (cache->getForwardMapping(stmtNum).empty()) {
        if (stmtNumToNodeMap.find(stmtNum) == stmtNumToNodeMap.end())
            return {};

        CFGNode *startNode = stmtNumToNodeMap.at(stmtNum);
        AdaptersUtils::runDownBFS(stmtNum, cache, startNode);
    }

    return cache->getForwardMapping(stmtNum);
}

unordered_set<string> AffectsKBAdapter::getAffectingT(const string &stmtNum) {
    if (!hasAffectsGraph(stmtNum)) {
        CFGNode *firstNode = AdaptersUtils::getStartingParentNode(pkb->getRootCFG(), stmtNum);
        buildAffectsGraphForProc(firstNode);
    }

    if (cache->getBackwardMapping(stmtNum).empty()) {
        if (stmtNumToNodeMap.find(stmtNum) == stmtNumToNodeMap.end())
            return {};

        CFGNode *endNode = stmtNumToNodeMap.at(stmtNum);
        AdaptersUtils::runUpBFS(stmtNum, cache, endNode);
    }

    return cache->getBackwardMapping(stmtNum);
}

unordered_set<string> AffectsKBAdapter::getAllStmtAffectingTOther() {
    if (!hasAffectsGraph())
        buildAffectsGraph();

    return affectings;
}

unordered_set<string> AffectsKBAdapter::getAllStmtAffectedTByOther() {
    if (!hasAffectsGraph())
        buildAffectsGraph();

    return affecteds;
}

vector<pair<string, string>> AffectsKBAdapter::getAffectsTAll() {
    if (!hasAffectsGraph())
        buildAffectsGraph();

    if (cache->getAllMapping().empty()) {
        CFGNode *start = stmtNumToNodeMap.at(ROOT_CFG);
        AdaptersUtils::fullBFS(cache, start);
    }

    return cache->getAllMapping();
}

//================================= Algo ==========================================

bool AffectsKBAdapter::bfsBool(CFGNode *start, const string &modifiedVar, const string &end) {
    queue<CFGNode *> queue;
    for (auto child: start->getChildren())
        queue.push(child);

    unordered_set<string> visited;

    while (!queue.empty()) {
        CFGNode *curr = queue.front();
        string stmtNum = curr->getStmtNum();
        vector<CFGNode *> children = curr->getChildren();
        queue.pop();

        // if current stmt uses this modifiedVar and is assign stmt
        if (stmtNum == end && pkb->isAssignStmt(stmtNum) && pkb->isUsesS(stmtNum, modifiedVar))
            return true;

        if (visited.find(stmtNum) != visited.end())
            continue;

        visited.insert(stmtNum);

        if (isModifyStmt(stmtNum) && pkb->isModifiesS(stmtNum, modifiedVar))
            continue;

        for (auto child: children)
            queue.push(child);
    }
    return false;
}

void AffectsKBAdapter::bfsDown(CFGNode *start, const string &modifiedVar, unordered_set<string> &affected) {
    queue<CFGNode *> queue;
    for (auto child: start->getChildren())
        queue.push(child);

    unordered_set<string> visited;

    while (!queue.empty()) {
        CFGNode *curr = queue.front();
        string stmtNum = curr->getStmtNum();
        vector<CFGNode *> children = curr->getChildren();
        queue.pop();

        // if current stmt uses this modifiedVar and is assign stmt
        if (pkb->isAssignStmt(stmtNum) && pkb->isUsesS(stmtNum, modifiedVar))
            affected.insert(stmtNum);

        if (visited.find(stmtNum) != visited.end())
            continue;

        visited.insert(stmtNum);

        if (isModifyStmt(stmtNum) && pkb->isModifiesS(stmtNum, modifiedVar))
            continue;

        for (auto child: children)
            queue.push(child);
    }
}

void AffectsKBAdapter::bfsUpSingle(CFGNode *start, const string &affectedVar, unordered_set<string> &affecting) {
    queue<CFGNode *> queue;
    for (auto parent: start->getParent()) {
        queue.push(parent);
    }

    unordered_set<string> visited;

    while (!queue.empty()) {
        CFGNode *curr = queue.front();
        string stmtNum = curr->getStmtNum();
        vector<CFGNode *> parents = curr->getParent();
        queue.pop();

        unordered_set<string> modifiedVarsInStmt = pkb->getModifiesByStmt(stmtNum);
        // if above is any of the 3 modify statement and is modifying the current affected var we dont need to explore
        if (isModifyStmt(stmtNum) && modifiedVarsInStmt.find(affectedVar) != modifiedVarsInStmt.end()) {
            if (pkb->isAssignStmt(stmtNum))
                affecting.insert(stmtNum);
            continue;
        }

        if (visited.find(stmtNum) != visited.end())
            continue;

        visited.insert(stmtNum);

        for (auto parent: parents)
            queue.push(parent);
    }
}

void AffectsKBAdapter::bfsUp(CFGNode *start, unordered_set<string> &affectedVars, unordered_set<string> &affecting) {
    for (auto &affectedVar: affectedVars)
        bfsUpSingle(start, affectedVar, affecting);
}

//================================= Affects Graph ==========================================

bool AffectsKBAdapter::hasAffectsGraph(const string &stmt) {
    return stmtNumToNodeMap.find(stmt) != stmtNumToNodeMap.end();
}

bool AffectsKBAdapter::hasAffectsGraph() const {
    return isAffectsGraphBuilt;
}

void AffectsKBAdapter::buildAffectsGraph() {
    CFGNode *root = pkb->getRootCFG();

    for (auto child: root->getChildren()) {
        if (!hasAffectsGraph(child->getStmtNum())) {
            buildAffectsGraphForProc(child);
        }
    }
    isAffectsGraphBuilt = true;
}

void AffectsKBAdapter::addAllStarting(CFGNode *node, queue<CFGNode *> &mainQ) {
    queue<CFGNode *> q;
    unordered_set<CFGNode *> visited;
    q.push(node);
    visited.insert(node);

    while (!q.empty()) {
        CFGNode *next = q.front();
        if (pkb->isAssignStmt(next->getStmtNum())) {
            CFGNode *affectNode = new CFGNode(next->getStmtNum());
            mainQ.push(affectNode);
            stmtNumToNodeMap.insert({next->getStmtNum(), affectNode});
            stmtNumToNodeMap.at(ROOT_CFG)->addChild(affectNode);
        }

        for (auto child: next->getChildren()) {
            if (visited.find(child) == visited.end()) {
                q.push(child);
                visited.insert(child);
            }
        }

        q.pop();
    }
}

void AffectsKBAdapter::buildAffectsGraphForProc(CFGNode *start) {
    queue<CFGNode *> mainQ;
    unordered_set<string> mainVisited;
    queue<CFGNode *> queue;
    unordered_set<string> visited;

    addAllStarting(start, mainQ);

    while (!mainQ.empty()) {
        CFGNode *affectingNode = mainQ.front();
        string affectingStmtNum = affectingNode->getStmtNum();
        vector<CFGNode *> nextChildren = pkb->getCFGForStmt(affectingStmtNum)->getChildren();

        for (auto child: nextChildren)
            queue.push(child);

        // mainQ only has assign stmt
        string modifiedVar = *pkb->getModifiesByStmt(affectingStmtNum).begin();

        while (!queue.empty()) {
            CFGNode *curr = queue.front();
            string stmtNum = curr->getStmtNum();
            vector<CFGNode *> children = curr->getChildren();
            queue.pop();

            // if current stmt uses this modifiedVar and is assign stmt
            if (pkb->isAssignStmt(stmtNum) && pkb->isUsesS(stmtNum, modifiedVar)) {
                CFGNode *childNode = stmtNumToNodeMap.at(stmtNum);
                affectingNode->addChild(childNode);
                childNode->addParent(affectingNode);
                affectingAffectedPairs.emplace_back(affectingStmtNum, stmtNum);
                affectings.insert(affectingStmtNum);
                affecteds.insert(stmtNum);
            }

            if (visited.find(stmtNum) != visited.end())
                continue;

            visited.insert(stmtNum);

            if (isModifyStmt(stmtNum) && pkb->isModifiesS(stmtNum, modifiedVar))
                continue;

            for (auto child: children)
                queue.push(child);
        }

        mainQ.pop();
        visited.clear();
    }
}
