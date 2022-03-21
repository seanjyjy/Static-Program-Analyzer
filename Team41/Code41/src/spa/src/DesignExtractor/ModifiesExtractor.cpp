#include "ModifiesExtractor.h"
#include "DesignExtractorUtils.h"
#include "Common/TNodeType.h"

#include <iostream>

ModifiesExtractor::ModifiesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap,
                                     unordered_map<string, unordered_set<string>> &callsMap, list<string> &procCallOrder) :
        ast(ast), nodeToStmtNumMap(nodeToStmtNumMap), callsMap(callsMap), procCallOrder(procCallOrder) {}

void ModifiesExtractor::mapModifies(TNode *node, unordered_set<string> &modifiesSet) {
    if (modifiesSet.empty()) return;

    if (node->getType() == TNodeType::procedure) {
        string procName = node->getTokenVal();
        auto it = procModifiesMap.find(procName);
        if (it == procModifiesMap.end()) // proc doesn't modify anything yet
            procModifiesMap.insert({node->getTokenVal(), modifiesSet});
        else // proc alr modifies something
            DesignExtractorUtils::copyOverSet(it->second, modifiesSet);
    } else {
        string stmtNum = nodeToStmtNumMap[node];
        auto it = stmtModifiesMap.find(stmtNum);
        if (it == stmtModifiesMap.end()) // stmt doesn't modify anything yet
            stmtModifiesMap.insert({stmtNum, modifiesSet});
        else // stmt alr modifies something
            DesignExtractorUtils::copyOverSet(it->second, modifiesSet);
    }
}

void ModifiesExtractor::dfs(TNode *node, unordered_set<string> &modifiesSet) {
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0], modifiesSet); // only 1 child stmtLst
        mapModifies(node, modifiesSet);
    } else if (type == TNodeType::stmtLst) {
        unordered_set<string> modifiesSetChild;
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) {
            dfs(child, modifiesSetChild);
            DesignExtractorUtils::combineSetsClear(modifiesSet, modifiesSetChild);
        }
    } else if (type == TNodeType::readStmt || type == TNodeType::assignStmt) {
        modifiesSet = {node->getChildren()[0]->getTokenVal()}; // left child varName
        mapModifies(node, modifiesSet);
    } else if (type == TNodeType::whileStmt) {
        dfs(node->getChildren()[1], modifiesSet); // right child stmtLst
        mapModifies(node, modifiesSet);
    } else if (type == TNodeType::ifStmt) {
        unordered_set<string> modifiesSetChild;
        vector<TNode *> ch = node->getChildren();
        for (size_t i = 1; i <= 2; i++) { // if stmt has stmtLst on 2nd and 3rd child
            dfs(ch[i], modifiesSetChild);
            DesignExtractorUtils::combineSetsClear(modifiesSet, modifiesSetChild);
        }
        mapModifies(node, modifiesSet);
    }
}

void ModifiesExtractor::buildProcModifiesCalls() {
    for (const string &procParent : procCallOrder) {
        for (const string& procChild : callsMap[procParent]) { // parent proc should always call a proc
            auto it = procModifiesMap.find(procChild);
            if (it == procModifiesMap.end()) continue; // child proc doesn't modify anything
            DesignExtractorUtils::copyOverSet(procModifiesMap[procParent], it->second);
        }
    }
}

void ModifiesExtractor::dfsCalls(TNode *node, unordered_set<string> &modifiesSet) {
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfsCalls(node->getChildren()[0], modifiesSet); // only 1 child stmtLst
    } else if (type == TNodeType::stmtLst) {
        unordered_set<string> modifiesSetChild;
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) {
            dfsCalls(child, modifiesSetChild);
            DesignExtractorUtils::combineSetsClear(modifiesSet, modifiesSetChild);
        }
    } else if (type == TNodeType::whileStmt) {
        dfsCalls(node->getChildren()[1], modifiesSet); // right child stmtLst
        mapModifies(node, modifiesSet);
    } else if (type == TNodeType::ifStmt) {
        unordered_set<string> modifiesSetChild;
        vector<TNode *> ch = node->getChildren();
        for (size_t i = 1; i <= 2; i++) { // if stmt has stmtLst on 2nd and 3rd child
            dfsCalls(ch[i], modifiesSetChild);
            DesignExtractorUtils::combineSetsClear(modifiesSet, modifiesSetChild);
        }
        mapModifies(node, modifiesSet);
    } else if (type == TNodeType::callStmt) {
        string procCalled = node->getChildren()[0]->getTokenVal();
        auto it = procModifiesMap.find(procCalled);
        if (it != procModifiesMap.end()) // procCalled modifies some variables
            modifiesSet = it->second;
        mapModifies(node, modifiesSet);
    }
}

void ModifiesExtractor::extractRelationship() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode : procNodes) { // map directly modifies first
        unordered_set<string> st;
        dfs(procNode, st);
    }
    buildProcModifiesCalls(); // build proc modifies due to calls
    for (TNode *procNode : procNodes) { // build stmt modifies due to calls
        unordered_set<string> st;
        dfsCalls(procNode, st);
    }
}

unordered_map<string, unordered_set<string>> ModifiesExtractor::getProcModifiesMap() {
    return this->procModifiesMap;
}

unordered_map<string, unordered_set<string>> ModifiesExtractor::getStmtModifiesMap() {
    return this->stmtModifiesMap;
}
