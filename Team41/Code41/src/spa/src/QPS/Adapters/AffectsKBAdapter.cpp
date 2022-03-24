#include "AffectsKBAdapter.h"

AffectsKBAdapter::AffectsKBAdapter(PKBClient *pkb) : pkb(pkb) { this->cache = new Cache(); }

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

    bool isAffects = bfs(start, variable, stmtNum2);
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

    bfs(start, variable, affected);
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

    bfs(start, affectedVars, affecting);
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

    if ((int) affectingAffectedPairs.size() == 0)
        buildAllAffectsMapping();

    return affectingAffectedPairs;
}

bool AffectsKBAdapter::isAffectsT(const string &stmtNum1, const string &stmtNum2) {
    if (!hasAffectsGraph())
        buildAffectsGraph();

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
    if (!hasAffectsGraph())
        buildAffectsGraph();

    if (cache->getForwardMapping(stmtNum).empty()) {
        if (stmtNumToNodeMap.find(stmtNum) == stmtNumToNodeMap.end())
            return {};

        CFGNode *startNode = stmtNumToNodeMap.at(stmtNum);
        AdaptersUtils::runDownBFS(stmtNum, cache, startNode);
    }

    return cache->getForwardMapping(stmtNum);
}

unordered_set<string> AffectsKBAdapter::getAffectingT(const string &stmtNum) {
    if (!hasAffectsGraph())
        buildAffectsGraph();

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

    if ((int) affectingAffectedTPairs.size() == 0)
        buildAllAffectsMapping();

    return affectingAffectedTPairs;
}

bool AffectsKBAdapter::bfs(CFGNode *start, const string &modifiedVar, const string &end) {
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

void AffectsKBAdapter::bfs(CFGNode *start, const string &modifiedVar, unordered_set<string> &affected) {
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

void AffectsKBAdapter::bfs(CFGNode *start, unordered_set<string> &affectedVars, unordered_set<string> &affecting) {
    queue<pair<CFGNode *, unordered_set<string>>> queue;
    for (auto parent: start->getParent()) {
        unordered_set<string> copy(affectedVars.begin(), affectedVars.end());
        queue.push({parent, copy});
    }

    unordered_set<string> visited;

    while (!queue.empty()) {
        auto[curr, unusedSet] = queue.front();
        string stmtNum = curr->getStmtNum();
        vector<CFGNode *> parents = curr->getParent();
        queue.pop();

        unordered_set<string> modifiedVarsInStmt = pkb->getModifiesByStmt(stmtNum);
        if (pkb->isCallStmt(stmtNum)) {
            unordered_set<string> temp;
            set_difference(unusedSet.begin(), unusedSet.end(), modifiedVarsInStmt.begin(), modifiedVarsInStmt.end(),
                           inserter(temp, temp.begin()));
            unusedSet = temp;
        }

        if (!modifiedVarsInStmt.empty()) {
            string modifiedVar = *(modifiedVarsInStmt).begin();
            if (unusedSet.find(modifiedVar) != unusedSet.end() && isModifyStmt(stmtNum)) {
                if (pkb->isAssignStmt(stmtNum))
                    affecting.insert(stmtNum);
                unusedSet.erase(modifiedVar);
            }
        }

        if (visited.find(stmtNum) != visited.end())
            continue;

        visited.insert(stmtNum);

        if (!unusedSet.empty()) {
            for (auto parent: parents) {
                unordered_set<string> copy(unusedSet.begin(), unusedSet.end());
                queue.push({parent, copy});
            }
        }
    }
}

bool AffectsKBAdapter::hasAffectsGraph() {
    return (int) stmtNumToNodeMap.size() != 0; // only when affectsGraph is built then size != 0
}

void AffectsKBAdapter::buildAffectsGraph() {
    CFGNode *root = pkb->getRootCFG();

    for (auto child: root->getChildren())
        buildAffectsGraphForProc(child);
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
        // TODO WHY local variable 'affectingNode' may point to invalidated memory
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

void AffectsKBAdapter::buildAffectsMapping(const string &stmtNum, CFGNode *node, unordered_set<CFGNode *> visited) {
    affectingAffectedTPairs.emplace_back(stmtNum, node->getStmtNum());
    for (auto child: node->getChildren()) {
        if (visited.find(child) != visited.end()) continue;
        visited.insert(child);
        buildAffectsMapping(stmtNum, child, visited);
        visited.erase(child);
    }
}

void AffectsKBAdapter::buildAllAffectsMapping() {
    unordered_set<CFGNode *> visited;
    for (auto [stmtNum, node]: stmtNumToNodeMap) {
        for (auto child: node->getChildren()) {
            affectingAffectedPairs.emplace_back(stmtNum, child->getStmtNum());
            buildAffectsMapping(stmtNum, child, visited);
        }
    }
}