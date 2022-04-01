#include "FollowsExtractor.h"
#include "Common/TNodeType.h"

FollowsExtractor::FollowsExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap) :
        StmtNumExtractor(ast, nodeToStmtNumMap) {}

void FollowsExtractor::mapFollows(TNode *node, list<string> &followsLst) {
    if (followsLst.empty()) return;
    followsTMap.insert({nodeToStmtNumMap.at(node), followsLst});
}

void FollowsExtractor::dfs(TNode *node) {
    TNodeType type = node->getType();
    if (type == TNodeType::procedure) {
        dfs(node->getChildren()[0]); // only 1 child stmtLst
    } else if (type == TNodeType::ifStmt) {
        const vector<TNode *> &ch = node->getChildren();
        dfs(ch[1]); // 2nd and 3rd child are stmtLst
        dfs(ch[2]);
    } else if (type == TNodeType::whileStmt) {
        dfs(node->getChildren()[1]); // right child is stmtLst
    } else if (type == TNodeType::stmtLst) {
        const vector<TNode *> &ch = node->getChildren();
        list<string> followsLst;
        for (int i = (int) ch.size() - 1; i >= 0; --i) {
            TNode *childNode = ch[i];
            dfs(childNode);
            mapFollows(childNode, followsLst);
            followsLst.push_front(nodeToStmtNumMap.at(childNode));
        }
    }
}

void FollowsExtractor::extract() {
    const vector<TNode *> &procNodes = ast->getChildren();
    for (TNode *procNode: procNodes) {
        dfs(procNode);
    }
}

unordered_map<string, list<string>> FollowsExtractor::getFollowsTMap() {
    return this->followsTMap;
}
