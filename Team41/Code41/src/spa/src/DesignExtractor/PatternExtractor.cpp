#include "PatternExtractor.h"
#include "Common/TNodeType.h"
#include "DesignExtractorUtils.h"

PatternExtractor::PatternExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap) :
        StmtNumExtractor(ast, nodeToStmtNumMap) {}

void PatternExtractor::mapAssignPattern(TNode *node) {
    const vector<TNode *> &ch = node->getChildren();
    pair<string, TNode *> lhsRhsPair = make_pair(ch[0]->getTokenVal(), ch[1]);
    assignPatternMap.insert({nodeToStmtNumMap.at(node), lhsRhsPair});
}

void PatternExtractor::mapIfPattern(TNode *node, unordered_set<string> &varSet) {
    if (varSet.empty()) return;
    ifPatternMap.insert({nodeToStmtNumMap.at(node), varSet});
}

void PatternExtractor::mapWhilePattern(TNode *node, unordered_set<string> &varSet) {
    if (varSet.empty()) return;
    whilePatternMap.insert({nodeToStmtNumMap.at(node), varSet});
}

void PatternExtractor::dfs(TNode *node) {
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0]); // only 1 child stmtLst
    } else if (type == TNodeType::ifStmt) {
        const vector<TNode *> &ch = node->getChildren();
        dfs(ch[1]); // 2nd and 3rd child are stmtLst
        dfs(ch[2]);
        unordered_set<string> vars;
        dfsExpr(ch[0], vars); // dfs on condition
        mapIfPattern(node, vars);
    } else if (type == TNodeType::whileStmt) {
        const vector<TNode *> &ch = node->getChildren();
        dfs(ch[1]); // dfs on stmtLst
        unordered_set<string> vars;
        dfsExpr(ch[0], vars); // dfs on condition
        mapWhilePattern(node, vars);
    } else if (type == TNodeType::stmtLst) {
        for (TNode *childNode: node->getChildren()) {
            dfs(childNode);
            if (childNode->getType() == TNodeType::assignStmt)
                mapAssignPattern(childNode);
        }
    }
}

void PatternExtractor::dfsExpr(TNode *node, unordered_set<string> &varSet) {
    unordered_set<string> varSetChild;
    TNodeType type = node->getType();
    if (isCondExpr(type) || isOp(type)) {
        for (TNode *child: node->getChildren()) {
            dfsExpr(child, varSetChild);
            DesignExtractorUtils::combineSetsClear(varSet, varSetChild);
        }
    } else if (type == TNodeType::varName) {
        varSet = {node->getTokenVal()};
    }
}

bool PatternExtractor::extract() {
    const vector<TNode *> &procNodes = ast->getChildren();
    for (TNode *procNode: procNodes) {
        dfs(procNode);
    }
    return true;
}

unordered_map<string, pair<string, TNode *>> PatternExtractor::getAssignPatternMap() {
    return this->assignPatternMap;
}

unordered_map<string, unordered_set<string>> PatternExtractor::getIfPatternMap() {
    return this->ifPatternMap;
}

unordered_map<string, unordered_set<string>> PatternExtractor::getWhilePatternMap() {
    return this->whilePatternMap;
}

