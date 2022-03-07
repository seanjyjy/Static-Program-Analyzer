#include "ModifiesExtractor.h"
#include "DesignExtractorUtils.h"
#include "Common/TNodeType.h"

ModifiesExtractor::ModifiesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap,
                                     unordered_map<string, unordered_set<string>> &callsMap, list<string> &procCallOrder) :
        ast(ast), nodeToStmtNumMap(nodeToStmtNumMap), callsMap(callsMap), procCallOrder(procCallOrder) {}

void ModifiesExtractor::mapModifies(TNode *node, unordered_set<string> &modifiesSet) {
    if (modifiesSet.empty()) return;

    if (node->getType() == TNodeType::procedure)
        procModifiesMap.insert({node->getTokenVal(), modifiesSet});
    else
        stmtModifiesMap.insert({nodeToStmtNumMap[node], modifiesSet});
}

void ModifiesExtractor::dfs(TNode *node, unordered_set<string> &modifiesSet) {
    unordered_set<string> modifiesSetChild;
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0], modifiesSetChild); // only 1 child stmtLst
        modifiesSet = modifiesSetChild;
        mapModifies(node, modifiesSet);
    } else if (type == TNodeType::stmtLst) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) {
            dfs(child, modifiesSetChild);
            DesignExtractorUtils::combineSetsClear(modifiesSet, modifiesSetChild);
        }
    } else if (type == TNodeType::readStmt || type == TNodeType::assignStmt) {
        modifiesSet = {node->getChildren()[0]->getTokenVal()}; // left child varName
        mapModifies(node, modifiesSet);
    } else if (type == TNodeType::whileStmt) {
        dfs(node->getChildren()[1], modifiesSetChild); // right child stmtLst
        modifiesSet = modifiesSetChild;
        mapModifies(node, modifiesSet);
    } else if (type == TNodeType::ifStmt) {
        vector<TNode *> ch = node->getChildren();
        for (size_t i = 1; i <= 2; i++) { // if stmt has stmtLst on 2nd and 3rd child
            dfs(ch[i], modifiesSetChild);
            DesignExtractorUtils::combineSetsClear(modifiesSet, modifiesSetChild);
        }
        mapModifies(node, modifiesSet);
    } else if (type == TNodeType::callStmt) {
        callNodeSet.insert(node);
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

void ModifiesExtractor::buildCallModifies() {
    for (TNode *callNode : callNodeSet) {
        string procCalled = callNode->getChildren()[0]->getTokenVal();
        auto it = procModifiesMap.find(procCalled);
        if (it == procModifiesMap.end()) continue; // proc called doesn't modify anything
        stmtModifiesMap[nodeToStmtNumMap[callNode]] = it->second;
    }
}

void ModifiesExtractor::extractRelationship() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode : procNodes) {
        unordered_set<string> st;
        dfs(procNode, st);
    }
    buildProcModifiesCalls();
    buildCallModifies();
}

unordered_map<string, unordered_set<string>> ModifiesExtractor::getProcModifiesMap() {
    return procModifiesMap;
}

unordered_map<string, unordered_set<string>> ModifiesExtractor::getStmtModifiesMap() {
    return stmtModifiesMap;
}
