#include "VarRelationExtractor.h"
#include "Common/AstNode/TNodeType.h"
#include "DesignExtractor/DesignExtractorUtils.h"

VarRelationExtractor::VarRelationExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap,
                                           unordered_map<string, unordered_set<string>> &callsMap,
                                           list<string> &procCallOrder) :
        StmtNumExtractor(ast, nodeToStmtNumMap), callsMap(callsMap), procCallOrder(procCallOrder) {}

void VarRelationExtractor::mapRelation(TNode *node, unordered_set<string> &relationSet) {
    if (relationSet.empty()) return;

    if (node->getType() == TNodeType::procedure) {
        string procName = node->getTokenVal();
        auto it = procRelationMap.find(procName);
        if (it == procRelationMap.end()) // proc doesn't <relation> anything yet
            procRelationMap.insert({node->getTokenVal(), relationSet});
        else // proc alr <relation> something
            DesignExtractorUtils::copyOverSet(it->second, relationSet);
    } else {
        string stmtNum = nodeToStmtNumMap.at(node);
        auto it = stmtRelationMap.find(stmtNum);
        if (it == stmtRelationMap.end()) // stmt doesn't <relation> anything yet
            stmtRelationMap.insert({stmtNum, relationSet});
        else // stmt alr <relation> something
            DesignExtractorUtils::copyOverSet(it->second, relationSet);
    }
}

void VarRelationExtractor::buildProcRelationCalls() {
    for (const string &procParent: procCallOrder) {
        for (const string &procChild: callsMap.at(procParent)) { // parent proc should always call a proc
            auto it = procRelationMap.find(procChild);
            if (it == procRelationMap.end()) continue; // child proc doesn't <relation> anything
            DesignExtractorUtils::copyOverSet(procRelationMap[procParent], it->second);
        }
    }
}

void VarRelationExtractor::dfsCalls(TNode *node, unordered_set<string> &relationSet) {
    if (node->isProcedure()) {
        dfsCalls(node->getChildren()[0], relationSet); // only 1 child stmtLst
    } else if (node->isStmtLst()) {
        unordered_set<string> usesSetChild;
        for (TNode *child: node->getChildren()) {
            dfsCalls(child, usesSetChild);
            DesignExtractorUtils::combineSetsClear(relationSet, usesSetChild);
        }
    } else if (node->isWhile()) {
        dfsCalls(node->getChildren()[1], relationSet); // right child stmtLst
        mapRelation(node, relationSet);
    } else if (node->isIf()) {
        unordered_set<string> usesSetChild;
        const vector<TNode *> &ch = node->getChildren();
        for (size_t i = 1; i <= 2; i++) { // if stmt has stmtLst on 2nd and 3rd child
            dfsCalls(ch[i], usesSetChild);
            DesignExtractorUtils::combineSetsClear(relationSet, usesSetChild);
        }
        mapRelation(node, relationSet);
    } else if (node->isCall()) {
        string procCalled = node->getChildren()[0]->getTokenVal();
        auto it = procRelationMap.find(procCalled);
        if (it != procRelationMap.end()) // procCalled <relation> some variables
            relationSet = it->second;
        mapRelation(node, relationSet);
    }
}

bool VarRelationExtractor::extract() {
    const vector<TNode *> &procNodes = ast->getChildren();
    for (TNode *procNode: procNodes) { // build stmt <relation> w/o calls
        unordered_set<string> st;
        dfs(procNode, st);
    }
    buildProcRelationCalls();
    for (TNode *procNode: procNodes) { // build stmt <relation> due to calls
        unordered_set<string> st;
        dfsCalls(procNode, st);
    }
    return true;
}

unordered_map<string, unordered_set<string>> VarRelationExtractor::getProcRelationMap() {
    return this->procRelationMap;
}

unordered_map<string, unordered_set<string>> VarRelationExtractor::getStmtRelationMap() {
    return this->stmtRelationMap;
}
