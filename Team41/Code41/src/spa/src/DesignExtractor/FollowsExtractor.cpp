#include "FollowsExtractor.h"
#include "Common/TNodeType.h"

FollowsExtractor::FollowsExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap) :
        ast(ast), nodeToStmtNumMap(nodeToStmtNumMap) {}

void FollowsExtractor::mapFollows(TNode *node, list<string> &followsLst) {
    if (followsLst.empty()) return;
    followsTMap.insert({nodeToStmtNumMap[node], followsLst});
}

void FollowsExtractor::dfs(TNode *node) {
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
        list<string> followsLst;
        for (int i = ch.size() - 1; i >= 0; i--) {
            TNode *childNode = ch[i];
            dfs(childNode);
            mapFollows(childNode, followsLst);
            followsLst.push_front(nodeToStmtNumMap[childNode]);
        }
    }
}

void FollowsExtractor::extractRelationship() {
    vector<TNode *> procNodes = ast->getChildren();
    for (TNode *procNode : procNodes) {
        dfs(procNode);
    }
}

unordered_map<string, list<string>> FollowsExtractor::getFollowsTMap() {
    return followsTMap;
}
