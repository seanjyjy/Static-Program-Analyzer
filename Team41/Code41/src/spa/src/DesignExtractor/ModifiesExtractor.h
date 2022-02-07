#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Common/TNode.h"

class ModifiesExtractor {
private:
    TNode *ast;
    unordered_map<TNode *, string> &nodeToStmtNumMap;
    unordered_map<string, unordered_set<string>> procModifiesMap;
    unordered_map<string, unordered_set<string>> stmtModifiesMap;

    void mapModifies(TNode *node, unordered_set<string> &modifiesSet);
    void dfs(TNode *node, unordered_set<string> &modifiesSet);

public:
    ModifiesExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap);

    void extractRelationship();

    unordered_map<string, unordered_set<string>> getProcModifiesMap();
    unordered_map<string, unordered_set<string>> getStmtModifiesMap();
};
