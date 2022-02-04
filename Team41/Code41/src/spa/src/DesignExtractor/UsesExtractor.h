#pragma once

#include <list>
#include <unordered_map>

#include "Common/TNode.h"
#include "PKB/PKB.h"

class UsesExtractor {
private:
    TNode *ast;
    PKB *pkb;
    unordered_map<TNode *, string> &nodeToStmtNumMap;

    void registerUses(TNode *node, list<TNode *> &usesLst);
    void dfs(TNode *node, list<TNode *> &usesLst);

public:
    UsesExtractor(TNode *ast, PKB *pkb, unordered_map<TNode *, string> &nodeToStmtNumMap);

    void extractRelationship();
};
