#include "PatternExtractor.h"
#include "Common/TNodeType.h"
#include "DesignExtractorUtils.h"

PatternExtractor::PatternExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap) :
        ast(ast), nodeToStmtNumMap(nodeToStmtNumMap) {}

void PatternExtractor::mapAssignPattern(TNode *node) {
    vector<TNode *> children = node->getChildren();
    pair<string, TNode *> lhsRhsPair = make_pair(children[0]->getTokenVal(), children[1]);
    assignPatternMap.insert({nodeToStmtNumMap[node], lhsRhsPair});
}

void PatternExtractor::mapIfPattern(TNode *node, unordered_set<string> &varSet) {
    if (varSet.empty()) return;
    ifPatternMap.insert({nodeToStmtNumMap[node], varSet});
}

void PatternExtractor::mapWhilePattern(TNode *node, unordered_set<string> &varSet) {
    if (varSet.empty()) return;
    whilePatternMap.insert({nodeToStmtNumMap[node], varSet});
}

void PatternExtractor::dfs(TNode *node) {
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0]); // only 1 child stmtLst
    } else if (type == TNodeType::ifStmt) {
        unordered_set<string> vars;
        dfsExpr(node, vars);
        mapIfPattern(node, vars);
        vector<TNode *> ch = node->getChildren();
        dfs(ch[1]); // 2nd and 3rd child are stmtLst
        dfs(ch[2]);
    } else if (type == TNodeType::whileStmt) {
        unordered_set<string> vars;
        dfsExpr(node, vars);
        mapWhilePattern(node, vars);
        dfs(node->getChildren()[1]); // right child is stmtLst
    } else if (type == TNodeType::stmtLst) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *childNode: ch) {
            dfs(childNode);
            if (childNode->getType() == TNodeType::assignStmt) {
                mapAssignPattern(childNode);
            }
        }
    }
}

void PatternExtractor::dfsExpr(TNode *node, unordered_set<string> &varSet) {
    unordered_set<string> varSetChild;
    TNodeType type = node->getType();
    if (isCondExpr(type) || isOp(type)) {
        vector<TNode *> ch = node->getChildren();
        for (TNode *child : ch) {
            dfsExpr(child, varSetChild);
            DesignExtractorUtils::combineSetsClear(varSet, varSetChild);
        }
    } else if (type == TNodeType::varName) {
        varSet = {node->getTokenVal()};
    }
}

void PatternExtractor::extractRelationship() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode: procNodes) {
        dfs(procNode);
    }
}

unordered_map<string, pair<string, TNode *>> PatternExtractor::getAssignPatternMap() {
    return assignPatternMap;
}

unordered_map<string, unordered_set<string>> PatternExtractor::getIfPatternMap() {
    return ifPatternMap;
}

unordered_map<string, unordered_set<string>> PatternExtractor::getWhilePatternMap() {
    return whilePatternMap;
}

