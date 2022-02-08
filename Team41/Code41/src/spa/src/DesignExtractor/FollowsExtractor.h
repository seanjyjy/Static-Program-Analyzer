#pragma once

#include <list>
#include <unordered_map>

#include "Common/TNode.h"

class FollowsExtractor {
private:
    TNode *ast;
    unordered_map<TNode *, string> &nodeToStmtNumMap;
    unordered_map<string, list<string>> followsTMap;

    void mapFollows(TNode *node, list<string> &followsLst);
    void dfs(TNode *node);

public:
    FollowsExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap);

    void extractRelationship();

    unordered_map<string, list<string>> getFollowsTMap();
};
