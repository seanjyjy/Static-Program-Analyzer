#pragma once

#include <list>
#include <unordered_map>

#include "Common/TNode.h"

class ParentExtractor {
private:
    TNode *ast;
    unordered_map<TNode *, string> &nodeToStmtNumMap;
    unordered_map<string, list<string>> parentMap;
    unordered_map<string, list<string>> parentTMap;

    void mapParent(TNode *node);
    void mapParentT(TNode *node, list<string> &parentLst);
    void dfs(TNode *node, list<string> &parentLst);

public:
    ParentExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap);

    void extractRelationship();

    unordered_map<string, list<string>> getParentMap();
    unordered_map<string, list<string>> getParentTMap();
};

