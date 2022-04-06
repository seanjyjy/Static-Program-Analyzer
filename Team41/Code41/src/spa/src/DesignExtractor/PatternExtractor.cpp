#include "PatternExtractor.h"
#include "Common/AstNode/TNodeType.h"
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
    if (node->isProcedure()) {
        dfs(node->getChildren()[0]); // only 1 child stmtLst
    } else if (node->isIf()) {
        const vector<TNode *> &ch = node->getChildren();
        dfs(ch[ifStmtLstFirst]); // 2nd and 3rd child are stmtLst
        dfs(ch[ifStmtLstSecond]);
        unordered_set<string> vars;
        dfsExpr(ch[condExpr], vars); // dfs on condition
        mapIfPattern(node, vars);
    } else if (node->isWhile()) {
        const vector<TNode *> &ch = node->getChildren();
        dfs(ch[whileStmtLst]); // dfs on stmtLst
        unordered_set<string> vars;
        dfsExpr(ch[condExpr], vars); // dfs on condition
        mapWhilePattern(node, vars);
    } else if (node->isStmtLst()) {
        for (TNode *childNode: node->getChildren()) {
            dfs(childNode);
            if (childNode->getType() == TNodeType::assignStmt)
                mapAssignPattern(childNode);
        }
    }
}

void PatternExtractor::dfsExpr(TNode *node, unordered_set<string> &varSet) {
    unordered_set<string> varSetChild;
    if (node->isCondExpr() || node->isArithmeticOp()) {
        for (TNode *child: node->getChildren()) {
            dfsExpr(child, varSetChild);
            DesignExtractorUtils::combineSetsClear(varSet, varSetChild);
        }
    } else if (node->isVarName()) {
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

