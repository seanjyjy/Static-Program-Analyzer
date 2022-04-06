#include "FollowsExtractor.h"

FollowsExtractor::FollowsExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap) :
        StmtNumExtractor(ast, nodeToStmtNumMap) {}

void FollowsExtractor::mapFollows(TNode *node, list<string> &followsLst) {
    if (followsLst.empty()) return;
    followsTMap.insert({nodeToStmtNumMap.at(node), followsLst});
}

void FollowsExtractor::dfs(TNode *node) {
    if (node->isProcedure()) {
        dfs(node->getChildren()[0]); // only 1 child stmtLst
    } else if (node->isIf()) {
        const vector<TNode *> &ch = node->getChildren();
        dfs(ch[ifStmtLstFirst]); // 2nd and 3rd child are stmtLst
        dfs(ch[ifStmtLstSecond]);
    } else if (node->isWhile()) {
        dfs(node->getChildren()[whileStmtLst]); // right child is stmtLst
    } else if (node->isStmtLst()) {
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

bool FollowsExtractor::extract() {
    const vector<TNode *> &procNodes = ast->getChildren();
    for (TNode *procNode: procNodes) {
        dfs(procNode);
    }
    return true;
}

unordered_map<string, list<string>> FollowsExtractor::getFollowsTMap() {
    return this->followsTMap;
}
