#include "AffectsKBProxy.h"

AffectsKBProxy::AffectsKBProxy(PKBClient *pkb) : pkb(pkb), cache(new Cache) {
    stmtNumToNodeMap.insert({ROOT_CFG, new CFGNode(ROOT_CFG)});
}

AffectsKBProxy::~AffectsKBProxy() {
    delete cache;
    CFGUtils::deleteCFG(stmtNumToNodeMap);
}

bool AffectsKBProxy::isAffects(const string &stmtNum1, const string &stmtNum2) {
    CFGNode *root = pkb->getRootCFG();
    CFGNode *firstNode = ProxyUtils::getStartingParentNode(root, stmtNum1);
    CFGNode *secondNode = ProxyUtils::getStartingParentNode(root, stmtNum2);
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

    string modifiedVar = *pkb->getModifiesByStmt(stmtNum1).begin();
    if (!pkb->isUsesS(stmtNum2, modifiedVar)) {
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

unordered_set<string> AffectsKBProxy::getDirectAffectsBy(const string &stmtNum) {
    if (!hasAffectsGraph(stmtNum)) {
        string modifiedVar = *pkb->getModifiesByStmt(stmtNum).begin();
        if (pkb->getUsesSByVar(modifiedVar).empty()) {
            return {};
        }
        return bfsDown(pkb, stmtNum);
    }

    unordered_set<string> affected;
    CFGNode *node = stmtNumToNodeMap.at(stmtNum);
    for (auto child: node->getChildren())
        affected.insert(child->getStmtNum());
    return affected;
}

unordered_set<string> AffectsKBProxy::getDirectAffecting(const string &stmtNum) {
    if (!hasAffectsGraph(stmtNum)) {
        return bfsUp(pkb, stmtNum);
    }

    unordered_set<string> affecting;
    CFGNode *node = stmtNumToNodeMap.at(stmtNum);
    for (auto parent: node->getParent())
        affecting.insert(parent->getStmtNum());
    return affecting;
}

unordered_set<string> AffectsKBProxy::getAllStmtAffectingOther() {
    if (!hasAffectsGraph())
        buildAffectsGraph();

    return affectings;
}

unordered_set<string> AffectsKBProxy::getAllStmtAffectedByOther() {
    if (!hasAffectsGraph())
        buildAffectsGraph();

    return affecteds;
}

bool AffectsKBProxy::isAffectingSomeStmt(const string &stmtNum) {
    if (hasAffectsGraph(stmtNum)) {
        CFGNode *node = stmtNumToNodeMap.at(stmtNum);
        return !node->getChildren().empty();
    }

    unordered_set<string> affected = bfsDown(pkb, stmtNum, true);
    return !affected.empty();
}

bool AffectsKBProxy::isAffectedBySomeStmt(const string &stmtNum) {
    if (hasAffectsGraph(stmtNum)) {
        CFGNode *node = stmtNumToNodeMap.at(stmtNum);
        return !node->getParent().empty();
    }

    unordered_set<string> affecting = bfsUp(pkb, stmtNum, true);
    return !affecting.empty();
}

bool AffectsKBProxy::hasSomeAffectsAll() {
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

vector<pair<string, string>> AffectsKBProxy::getDirectAffectsAll() {
    if (!hasAffectsGraph())
        buildAffectsGraph();

    return affectingAffectedPairs;
}

bool AffectsKBProxy::isAffectsT(const string &stmtNum1, const string &stmtNum2) {
    CFGNode *root = pkb->getRootCFG();
    CFGNode *firstCFGNode = ProxyUtils::getStartingParentNode(root, stmtNum1);
    CFGNode *secondCFGNode = ProxyUtils::getStartingParentNode(root, stmtNum2);
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
        ProxyUtils::runBoolBFS(stmtNum1, stmtNum2, cache, startNode);
    }

    return cache->getBooleanMapping(stmtNum1, stmtNum2);
}

unordered_set<string> AffectsKBProxy::getAffectsTBy(const string &stmtNum) {
    if (!hasAffectsGraph(stmtNum)) {
        CFGNode *firstNode = ProxyUtils::getStartingParentNode(pkb->getRootCFG(), stmtNum);
        buildAffectsGraphForProc(firstNode);
    }

    if (cache->getForwardMapping(stmtNum).empty()) {
        CFGNode *startNode = stmtNumToNodeMap.at(stmtNum);
        ProxyUtils::runDownBFS(stmtNum, cache, startNode);
    }

    return cache->getForwardMapping(stmtNum);
}

unordered_set<string> AffectsKBProxy::getAffectingT(const string &stmtNum) {
    if (!hasAffectsGraph(stmtNum)) {
        CFGNode *firstNode = ProxyUtils::getStartingParentNode(pkb->getRootCFG(), stmtNum);
        buildAffectsGraphForProc(firstNode);
    }

    if (cache->getBackwardMapping(stmtNum).empty()) {
        CFGNode *endNode = stmtNumToNodeMap.at(stmtNum);
        ProxyUtils::runUpBFS(stmtNum, cache, endNode);
    }

    return cache->getBackwardMapping(stmtNum);
}

unordered_set<string> AffectsKBProxy::getAllStmtAffectingTOther() {
    if (!hasAffectsGraph())
        buildAffectsGraph();

    return affectings;
}

unordered_set<string> AffectsKBProxy::getAllStmtAffectedTByOther() {
    if (!hasAffectsGraph())
        buildAffectsGraph();

    return affecteds;
}

vector<pair<string, string>> AffectsKBProxy::getAffectsTAll() {
    if (!hasAffectsGraph())
        buildAffectsGraph();

    if (cache->getAllMapping().empty()) {
        CFGNode *start = stmtNumToNodeMap.at(ROOT_CFG);
        ProxyUtils::fullBFS(cache, start);
    }

    return cache->getAllMapping();
}

//================================= Algo ==========================================

const unordered_set<string> &AffectsKBProxy::getAllParentStmt() {
    if (cachedParentStmts.empty())
        cachedParentStmts = pkb->getAllStmtsParentOfSomeStmt();
    return cachedParentStmts;
}

const unordered_set<string> &AffectsKBProxy::getAllAssignStmt() {
    if (cachedAssignStmts.empty())
        cachedAssignStmts = pkb->getAssigns();
    return cachedAssignStmts;
}

bool AffectsKBProxy::bfsBool(PKBClient *client, CFGNode *start, const string &modifiedVar, const string &end) {
    bool canReach = false;
    unordered_set<string> parentStmts = getAllParentStmt();
    CacheCallback shouldContinue = [&modifiedVar, &client, &parentStmts](const string &next) {
        return !client->isModifiesS(next, modifiedVar) || parentStmts.find(next) != parentStmts.end();
    };

    TerminateCheck canEnd = [&end, &canReach, &modifiedVar, &client](CFGNode *next) {
        string stmtNum = next->getStmtNum();
        if (stmtNum == end && client->isAssignStmt(stmtNum) && client->isUsesS(stmtNum, modifiedVar)) {
            canReach = true;
        }
        return canReach;
    };

    ProxyUtils::runBFS(true, start, shouldContinue, canEnd);
    return canReach;
}

unordered_set<string> AffectsKBProxy::bfsDown(PKBClient *client, const string &stmtNum, bool isAnyResult) {
    unordered_set<string> affected;
    CFGNode *start = client->getCFGForStmt(stmtNum);
    // we are ensured that there is a modified var since this is an assignment statement
    string modifiedVar = *client->getModifiesByStmt(stmtNum).begin();
    unordered_set<string> parentStmts = getAllParentStmt();
    unordered_set<string> assignStmts = getAllAssignStmt();

    CacheCallback shouldContinue = [&client, &modifiedVar, &affected, &parentStmts, &assignStmts](const string &next) {
        // if current stmt uses this modifiedVar and is assign stmt
        if (assignStmts.find(next) != assignStmts.end() && client->isUsesS(next, modifiedVar))
            affected.insert(next);

        // check that affects is still maintained
        return parentStmts.find(next) != parentStmts.end() || !client->isModifiesS(next, modifiedVar);
    };

    TerminateCheck canEnd = [&affected, &isAnyResult](CFGNode *) { return isAnyResult && !affected.empty(); };

    ProxyUtils::runBFS(true, start, shouldContinue, canEnd);
    return affected;
}

unordered_set<string> AffectsKBProxy::bfsUp(PKBClient *client, const string &stmtNum, bool isAnyResult) {
    unordered_set<string> affecting;
    CFGNode *start = pkb->getCFGForStmt(stmtNum);
    unordered_set<string> affectedVars = pkb->getUsesByStmt(stmtNum);
    unordered_set<string> parentStmts = getAllParentStmt();
    unordered_set<string> assignStmts = getAllAssignStmt();

    TerminateCheck canEnd = [&affecting, &isAnyResult](CFGNode *) { return isAnyResult && !affecting.empty(); };

    for (auto &affectedVar: affectedVars) {
        CacheCallback shouldContinue = [&affecting, &client, &affectedVar, &parentStmts, &assignStmts](
                const string &next) {
            unordered_set<string> modifiedVarsInStmt = client->getModifiesByStmt(next);

            // if above is an if/while statement and is modifying the current affected var we need to explore
            if (modifiedVarsInStmt.find(affectedVar) == modifiedVarsInStmt.end() ||
                parentStmts.find(next) != parentStmts.end())
                return true;

            if (assignStmts.find(next) != assignStmts.end())
                affecting.insert(next);

            return false;
        };

        ProxyUtils::runBFS(false, start, shouldContinue, canEnd);
    }
    return affecting;
}

//================================= Affects Graph ==========================================

bool AffectsKBProxy::hasAffectsGraph(const string &stmt) const {
    return stmtNumToNodeMap.find(stmt) != stmtNumToNodeMap.end();
}

bool AffectsKBProxy::hasAffectsGraph() const {
    return isAffectsGraphBuilt;
}

void AffectsKBProxy::buildAffectsGraph() {
    CFGNode *root = pkb->getRootCFG();

    for (auto child: root->getChildren()) {
        if (!hasAffectsGraph(child->getStmtNum())) {
            buildAffectsGraphForProc(child);
        }
    }
    isAffectsGraphBuilt = true;
}

void AffectsKBProxy::addAllStarting(CFGNode *node, queue<CFGNode *> &mainQ) {
    queue<CFGNode *> q;
    unordered_set<CFGNode *> visited;
    q.push(node);
    visited.insert(node);
    getAllAssignStmt();

    while (!q.empty()) {
        CFGNode *next = q.front();
        string stmtNum = next->getStmtNum();
        if (cachedAssignStmts.find(stmtNum) != cachedAssignStmts.end()) {
            CFGNode *affectNode = new CFGNode(stmtNum);
            mainQ.push(affectNode);
            stmtNumToNodeMap.insert({stmtNum, affectNode});
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

void AffectsKBProxy::buildAffectsGraphForProc(CFGNode *start) {
    queue<CFGNode *> mainQ;
    queue<CFGNode *> queue;
    getAllParentStmt();
    getAllAssignStmt();

    addAllStarting(start, mainQ);

    while (!mainQ.empty()) {
        unordered_set<string> visited;
        CFGNode *affectingNode = mainQ.front();
        string affectingStmtNum = affectingNode->getStmtNum();
        const vector<CFGNode *> &nextChildren = pkb->getCFGForStmt(affectingStmtNum)->getChildren();

        // mainQ only has assign stmt
        string modifiedVar = *pkb->getModifiesByStmt(affectingStmtNum).begin();

        // terminate if no one uses it, checking is O(1)
        if (pkb->getUsesSByVar(modifiedVar).empty()) {
            mainQ.pop();
            continue;
        }

        for (auto child: nextChildren)
            queue.push(child);

        while (!queue.empty()) {
            CFGNode *curr = queue.front();
            string stmtNum = curr->getStmtNum();
            const vector<CFGNode *> &children = curr->getChildren();
            queue.pop();

            if (visited.find(stmtNum) != visited.end())
                continue;

            visited.insert(stmtNum);

            // if current stmt uses this modifiedVar and is assign stmt
            if (cachedAssignStmts.find(stmtNum) != cachedAssignStmts.end() && pkb->isUsesS(stmtNum, modifiedVar)) {
                CFGNode *childNode = stmtNumToNodeMap.at(stmtNum);
                affectingNode->addChild(childNode);
                childNode->addParent(affectingNode);
                affectingAffectedPairs.emplace_back(affectingStmtNum, stmtNum);
                affectings.insert(affectingStmtNum);
                affecteds.insert(stmtNum);
            }

            if (cachedParentStmts.find(stmtNum) == cachedParentStmts.end() && pkb->isModifiesS(stmtNum, modifiedVar))
                continue;

            for (auto child: children)
                queue.push(child);
        }

        mainQ.pop();
    }
}
