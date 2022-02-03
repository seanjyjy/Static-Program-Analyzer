#pragma once

#include <list>

#include "Common/TNode.h"
#include "PKB/PKB.h"

class ModifiesExtractor {
private:
    TNode *ast;
    PKB *pkb;
    unordered_map<TNode *, string> &nodeToStmtNumMap;

    void registerModifies(TNode *node, list<TNode *> &modifiesLst);
    void dfs(TNode *node, list<TNode *> &modifiesLst);

public:
    ModifiesExtractor(TNode *ast, PKB *pkb, unordered_map<TNode *, string> &nodeToStmtNumMap);

    void extractRelationship();
};
