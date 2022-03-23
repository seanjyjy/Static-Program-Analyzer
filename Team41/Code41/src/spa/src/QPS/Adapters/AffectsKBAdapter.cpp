#include "AffectsKBAdapter.h"

AffectsKBAdapter::AffectsKBAdapter(PKBClient *pkb): pkb(pkb) { this->cache = new Cache(); }

AffectsKBAdapter::~AffectsKBAdapter() { delete cache; }

bool AffectsKBAdapter::isModifyStmt(const string& stmtNum) {
    return pkb->isAssignStmt(stmtNum) || pkb->isCallStmt(stmtNum) || pkb->isReadStmt(stmtNum);
}

bool AffectsKBAdapter::isAffects(const string &stmt1, const string &stmt2) {
    if (hasAffectsGraph()) {
        CFGNode* node = stmtNumToNodeMap.at(stmt1);
        for (auto& child : node->getChildren()) {
            if (child->getStmtNum() == stmt2)
                return true;
        }
        return false;
    }

    CFGNode* start = pkb->getCFGForStmt(stmt1);
    string variable = *pkb->getModifiesByStmt(stmt1).begin();

    unordered_set<string> variablesUsed = pkb->getUsesByStmt(stmt2);
    // the variable that was modified in stmt1 isnt being used in stmt2
    if (variablesUsed.find(variable) == variablesUsed.end())
        return false;

    bool isAffects = bfs(start, variable, stmt2);
    return isAffects;
}

unordered_set<string> AffectsKBAdapter::getDirectAffectsBy(const string &stmt) {
    unordered_set<string> affected;

    if (hasAffectsGraph()) {
        CFGNode* node = stmtNumToNodeMap.at(stmt);
        for (auto& child : node->getChildren())
            affected.insert(child->getStmtNum());
        return affected;
    }

    CFGNode* start = pkb->getCFGForStmt(stmt);
    // we are ensured that there is a modified var since this is an assignment statement
    string variable = *pkb->getModifiesByStmt(stmt).begin();

    bfs(start, variable, affected);
    return affected;
}

unordered_set<string> AffectsKBAdapter::getDirectAffecting(const string &stmt) {
    unordered_set<string> affecting;

    if (hasAffectsGraph()) {
        CFGNode* node = stmtNumToNodeMap.at(stmt);
        for (auto& child : node->getChildren())
            affecting.insert(child->getStmtNum());
        return affecting;
    }

    CFGNode* start = pkb->getCFGForStmt(stmt);
    unordered_set<string> affectedVars = pkb->getUsesByStmt(stmt);

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
    return affectingAffectedPairs;
}

bool AffectsKBAdapter::bfs(CFGNode *start, const string &modifiedVar, const string& end) {
    queue<CFGNode *> queue;
    for (auto& child : start->getChildren())
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

        for (auto& child : children)
            queue.push(child);
    }
    return false;
}

void AffectsKBAdapter::bfs(CFGNode *start, const string &modifiedVar, unordered_set<string> &affected) {
    queue<CFGNode *> queue;
    for (auto& child : start->getChildren())
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

        for (auto& child : children)
            queue.push(child);
    }
}

void AffectsKBAdapter::bfs(CFGNode *start, unordered_set<string> &affectedVars, unordered_set<string> &affecting) {
    queue<pair<CFGNode *, unordered_set<string>>> queue;
    for (auto& parent : start->getParent()) {
        unordered_set<string> copy(affectedVars.begin(), affectedVars.end());
        queue.push({parent, copy});
    }

    unordered_set<string> visited;

    while (!queue.empty()) {
        auto[curr, unusedSet] = queue.front();
        string stmtNum = curr->getStmtNum();
        vector<CFGNode *> parents = curr->getParent();
        queue.pop();

        string modifiedVar = *pkb->getModifiesByStmt(stmtNum).begin();
        if (unusedSet.find(modifiedVar) != unusedSet.end() && isModifyStmt(stmtNum)) {
            if (pkb->isAssignStmt(stmtNum))
                affecting.insert(stmtNum);
            unusedSet.erase(modifiedVar);
        }

        if (visited.find(stmtNum) != visited.end())
            continue;

        visited.insert(stmtNum);

        if (!unusedSet.empty()) {
            for (auto& parent : parents) {
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
    CFGNode* root = pkb->getRootCFG();

    for (auto& child : root->getChildren()) {
        CFGNode* affectsGraphForProc = buildAffectsGraphForProc(child);
        if (affectsGraphForProc != nullptr)
            affectsGraph->addChild(affectsGraphForProc);
    }
}

void AffectsKBAdapter::addAllStarting(CFGNode* node, queue<CFGNode *> &mainQ) {
    queue<CFGNode *> q;
    unordered_set<CFGNode *> visited;
    q.push(node);
    visited.insert(node);

    while (!q.empty()) {
        CFGNode* next = q.front();
        if (pkb->isAssignStmt(next->getStmtNum())) {
            CFGNode* affectNode = new CFGNode(next->getStmtNum());
            mainQ.push(affectNode);
            stmtNumToNodeMap.insert({next->getStmtNum(), affectNode});
        }

        for (auto& child : next->getChildren()) {
            if (visited.find(child) == visited.end()) {
                q.push(child);
                visited.insert(child);
            }
        }

        q.pop();
    }
}

CFGNode* AffectsKBAdapter::buildAffectsGraphForProc(CFGNode* start) {
    queue<CFGNode* > mainQ;
    unordered_set<string> mainVisited;
    queue<CFGNode *> queue;
    unordered_set<string> visited;

    addAllStarting(start, mainQ);

    // Group Representative
    CFGNode* procRoot = nullptr;

    while (!mainQ.empty()) {
        CFGNode* affectingNode = mainQ.front();
        // TODO WHY local variable 'affectingNode' may point to invalidated memory
        string affectingStmtNum = affectingNode->getStmtNum();
        vector<CFGNode *> nextChildren = pkb->getCFGForStmt(affectingStmtNum)->getChildren();

        if (procRoot == nullptr)
            procRoot = affectingNode;

        for (auto& child : nextChildren)
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
                affectingAffectedPairs.emplace_back(affectingStmtNum, stmtNum);
            }

            if (visited.find(stmtNum) != visited.end())
                continue;

            visited.insert(stmtNum);

            if (isModifyStmt(stmtNum) && pkb->isModifiesS(stmtNum, modifiedVar))
                continue;

            for (auto& child : children)
                queue.push(child);
        }

        mainQ.pop();
        visited.clear();
    }

    return procRoot;
}