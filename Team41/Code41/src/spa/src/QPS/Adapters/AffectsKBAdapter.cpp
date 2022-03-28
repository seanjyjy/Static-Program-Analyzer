#include "AffectsKBAdapter.h"

AffectsKBAdapter::AffectsKBAdapter(PKBClient *pkb) : pkb(pkb), cache(new Cache) {
    stmtNumToNodeMap.insert({ROOT_CFG, new CFGNode(ROOT_CFG)});
}

AffectsKBAdapter::~AffectsKBAdapter() { delete cache; }

bool AffectsKBAdapter::isAffects(const string &stmtNum1, const string &stmtNum2) {
    CFGNode *root = pkb->getRootCFG();
    CFGNode *firstNode = AdaptersUtils::getStartingParentNode(root, stmtNum1);
    CFGNode *secondNode = AdaptersUtils::getStartingParentNode(root, stmtNum2);
    if (firstNode == nullptr || firstNode != secondNode) {
        return false;
    }
    if (hasAffectsGraph(stmtNum1)) {
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

    return bfsBool(pkb, start, variable, stmtNum2);
}

unordered_set<string> AffectsKBAdapter::getDirectAffectsBy(const string &stmtNum) {
    if (!hasAffectsGraph(stmtNum)) {
        return bfsDown(pkb, stmtNum);
    }

    unordered_set<string> affected;
    CFGNode *node = stmtNumToNodeMap.at(stmtNum);
    for (auto child: node->getChildren())
        affected.insert(child->getStmtNum());
    return affected;
}

unordered_set<string> AffectsKBAdapter::getDirectAffecting(const string &stmtNum) {
    if (!hasAffectsGraph(stmtNum)) {
        return bfsUp(pkb, stmtNum);
    }

    unordered_set<string> affecting;
    CFGNode *node = stmtNumToNodeMap.at(stmtNum);
    for (auto parent: node->getParent())
        affecting.insert(parent->getStmtNum());
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

bool AffectsKBAdapter::isAffectingSomeStmt(const string &stmtNum) {
    if (hasAffectsGraph(stmtNum)) {
        CFGNode *node = stmtNumToNodeMap.at(stmtNum);
        return !node->getChildren().empty();
    }

    unordered_set<string> affected = bfsDown(pkb, stmtNum, true);
    return !affected.empty();
}

bool AffectsKBAdapter::isAffectedBySomeStmt(const string &stmtNum) {
    if (hasAffectsGraph(stmtNum)) {
        CFGNode *node = stmtNumToNodeMap.at(stmtNum);
        return !node->getParent().empty();
    }

    unordered_set<string> affecting = bfsUp(pkb, stmtNum, true);
    return !affecting.empty();
}

bool AffectsKBAdapter::hasSomeAffectsAll() {
    if (!affectingAffectedPairs.empty()) {
        return true;
    }

    CFGNode *root = pkb->getRootCFG();
    for (auto child: root->getChildren()) {
        if (hasAffectsGraph(child->getStmtNum())) {
            continue;
        }
        buildAffectsGraphForProc(child);
        if (!affectingAffectedPairs.empty()) {
            return true;
        }
    }
    return false;
}

vector<pair<string, string>> AffectsKBAdapter::getDirectAffectsAll() {
    if (!hasAffectsGraph())
        buildAffectsGraph();

    return affectingAffectedPairs;
}

bool AffectsKBAdapter::isAffectsT(const string &stmtNum1, const string &stmtNum2) {
    CFGNode *root = pkb->getRootCFG();
    CFGNode *firstCFGNode = AdaptersUtils::getStartingParentNode(root, stmtNum1);
    CFGNode *secondCFGNode = AdaptersUtils::getStartingParentNode(root, stmtNum2);
    if (firstCFGNode == nullptr || firstCFGNode != secondCFGNode) {
        return false;
    }

    if (!hasAffectsGraph(stmtNum1)) {
        buildAffectsGraphForProc(firstCFGNode);
    }

    unordered_set<string> backwardCache = cache->getBackwardMapping(stmtNum2);
    if (!backwardCache.empty())
        return backwardCache.find(stmtNum1) != backwardCache.end();

    unordered_set<string> forwardCache = cache->getForwardMapping(stmtNum1);
    if (!forwardCache.empty())
        return forwardCache.find(stmtNum2) != forwardCache.end();

    if (!cache->getBooleanMapping(stmtNum1, stmtNum2)) {
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

bool AffectsKBAdapter::isModifyStmt(PKBClient *pkb, const string &stmtNum) {
    return pkb->isAssignStmt(stmtNum) || pkb->isCallStmt(stmtNum) || pkb->isReadStmt(stmtNum);
}

bool AffectsKBAdapter::bfsBool(PKBClient *client, CFGNode *start, const string &modifiedVar, const string &end) {
    bool canReach = false;

    CacheCallback shouldContinue = [&modifiedVar, &client](const string &next) {
        return !client->isModifiesS(next, modifiedVar) || !isModifyStmt(client, next);
    };

    TerminateCheck canEnd = [&end, &canReach, &modifiedVar, &client](CFGNode *next) {
        string stmtNum = next->getStmtNum();
        if (stmtNum == end && client->isAssignStmt(stmtNum) && client->isUsesS(stmtNum, modifiedVar)) {
            canReach = true;
        }
        return canReach;
    };

    AdaptersUtils::runBFS(true, shouldContinue, canEnd, start);
    return canReach;
}

unordered_set<string> AffectsKBAdapter::bfsDown(PKBClient *client, const string& stmtNum, bool isAnyResult) {
    unordered_set<string> affected;
    CFGNode *start = client->getCFGForStmt(stmtNum);
    // we are ensured that there is a modified var since this is an assignment statement
    string modifiedVar = *client->getModifiesByStmt(stmtNum).begin();

    CacheCallback shouldContinue = [&client, &modifiedVar, &affected](const string &next) {
        // if current stmt uses this modifiedVar and is assign stmt
        if (client->isAssignStmt(next) && client->isUsesS(next, modifiedVar))
            affected.insert(next);

        // check that affects is still maintained
        return !client->isModifiesS(next, modifiedVar) || !isModifyStmt(client, next);
    };

    TerminateCheck canEnd = [&affected, &isAnyResult](CFGNode *) { return isAnyResult && !affected.empty(); };

    AdaptersUtils::runBFS(true, shouldContinue, canEnd, start);
    return affected;
}

unordered_set<string> AffectsKBAdapter::bfsUp(PKBClient *client, const string& stmtNum, bool isAnyResult) {
    unordered_set<string> affecting;
    CFGNode *start = pkb->getCFGForStmt(stmtNum);
    unordered_set<string> affectedVars = pkb->getUsesByStmt(stmtNum);

    TerminateCheck canEnd = [&affecting, &isAnyResult](CFGNode *) { return isAnyResult && !affecting.empty(); };

    for (auto &affectedVar: affectedVars) {
        CacheCallback shouldContinue = [&affecting, &client, &affectedVar](const string &next) {
            unordered_set<string> modifiedVarsInStmt = client->getModifiesByStmt(next);

            // if above is any of the 3 modify statement and is modifying the current affected var we dont need to explore
            if (!isModifyStmt(client, next) || modifiedVarsInStmt.find(affectedVar) == modifiedVarsInStmt.end())
                return true;

            if (client->isAssignStmt(next))
                affecting.insert(next);

            return false;
        };

        AdaptersUtils::runBFS(false, shouldContinue, canEnd, start);
    }
    return affecting;
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

            if (isModifyStmt(pkb, stmtNum) && pkb->isModifiesS(stmtNum, modifiedVar))
                continue;

            for (auto child: children)
                queue.push(child);
        }

        mainQ.pop();
        visited.clear();
    }
}
