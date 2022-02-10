#include "PatternExtractor.h"
#include "Common/TNodeType.h"

PatternExtractor::PatternExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap) :
        ast(ast), nodeToStmtNumMap(nodeToStmtNumMap) {}

void PatternExtractor::mapAssignPattern(TNode *node) {
    vector<TNode *> children = node->getChildren();
    pair<string, TNode *> lhsRhsPair = make_pair(children[0]->getVal()->getVal(), children[1]);
    assignPatternMap.insert({nodeToStmtNumMap[node], lhsRhsPair});
}

void PatternExtractor::dfs(TNode *node) {
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0]); // only 1 child stmtLst
    } else if (type == TNodeType::ifStmt) {
        vector<TNode *> ch = node->getChildren();
        dfs(ch[1]); // 2nd and 3rd child are stmtLst
        dfs(ch[2]);
    } else if (type == TNodeType::whileStmt) {
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

void PatternExtractor::extractRelationship() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode: procNodes) {
        dfs(procNode);
    }
}

unordered_map<string, pair<string, TNode *>> PatternExtractor::getAssignPatternMap() {
    return assignPatternMap;
}
