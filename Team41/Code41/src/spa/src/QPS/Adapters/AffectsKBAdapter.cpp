#include "AffectsKBAdapter.h"

AffectsKBAdapter::AffectsKBAdapter(PKBClient *pkb): pkb(pkb) { this->cache = new Cache(); }

AffectsKBAdapter::~AffectsKBAdapter() { delete cache; }

bool AffectsKBAdapter::isModifyStmt(const string& stmtNum) {
    return pkb->isAssignStmt(stmtNum) || pkb->isCallStmt(stmtNum) || pkb->isReadStmt(stmtNum);
}

bool AffectsKBAdapter::bfs(CFGNode *start, const string &modifiedVar, const string& end) {
    queue<CFGNode *> queue;
    for (auto& child : start->getChildren()) {
        queue.push(child);
    }

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

        if (pkb->isReadStmt(stmtNum) || pkb->isAssignStmt(stmtNum) || pkb->isCallStmt(stmtNum)) {
            if (pkb->isModifiesS(stmtNum, modifiedVar)) {
                continue;
            }
        }

        for (auto& child : children) {
            queue.push(child);
        }
    }
    return false;
}

bool AffectsKBAdapter::isAffects(const string &stmt1, const string &stmt2) {
    CFGNode* start = pkb->getCFGForStmt(stmt1);
    string variable = *pkb->getModifiesByStmt(stmt1).begin();

    unordered_set<string> variablesUsed = pkb->getUsesByStmt(stmt2);
    // the variable that was modified in stmt1 isnt being used in stmt2
    if (variablesUsed.find(variable) == variablesUsed.end()) {
        return false;
    }

    bool isAffects = bfs(start, variable, stmt2);
    return isAffects;
}

void AffectsKBAdapter::bfs(CFGNode *start, const string &modifiedVar, unordered_set<string> &affected) {
    queue<CFGNode *> queue;
    for (auto& child : start->getChildren()) {
        queue.push(child);
    }

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

        if (isModifyStmt(stmtNum) && pkb->isModifiesS(stmtNum, modifiedVar)) {
            continue;
        }

        for (auto& child : children) {
            queue.push(child);
        }
    }
}

unordered_set<string> AffectsKBAdapter::getDirectAffectsBy(const string &stmt) {
    unordered_set<string> affected;
    CFGNode* start = pkb->getCFGForStmt(stmt);
    string variable = *pkb->getModifiesByStmt(stmt).begin();

    bfs(start, variable, affected);
    return affected;
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
            if (pkb->isAssignStmt(stmtNum)) {
                affecting.insert(stmtNum);
            }
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

unordered_set<string> AffectsKBAdapter::getDirectAffecting(const string &stmt) {
    unordered_set<string> affecting;
    CFGNode* start = pkb->getCFGForStmt(stmt);
    unordered_set<string> affectedVars = pkb->getUsesByStmt(stmt);

    bfs(start, affectedVars, affecting);
    return affecting;
}

unordered_set<string> AffectsKBAdapter::getAllStmtAffectingOther() {
    return unordered_set<string>();
}

unordered_set<string> AffectsKBAdapter::getAllStmtAffectedByOther() {
    return unordered_set<string>();
}

vector<pair<string, string>> AffectsKBAdapter::getDirectAffectsAll() {
    return vector<pair<string, string>>();
}
