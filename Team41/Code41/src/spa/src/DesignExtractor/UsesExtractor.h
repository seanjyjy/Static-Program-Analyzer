#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Common/TNode.h"

class UsesExtractor {
private:
    TNode *ast;
    unordered_map<TNode *, string> &nodeToStmtNumMap;
    unordered_map<string, unordered_set<string>> procUsesMap;
    unordered_map<string, unordered_set<string>> stmtUsesMap;

    void mapUses(TNode *node, unordered_set<string> &usesSet);
    void dfs(TNode *node, unordered_set<string> &usesSet);

public:
    UsesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap);

    void extractRelationship();

    unordered_map<string, unordered_set<string>> getProcUsesMap();
    unordered_map<string, unordered_set<string>> getStmtUsesMap();
};
