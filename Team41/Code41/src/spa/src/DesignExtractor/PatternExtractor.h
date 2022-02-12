#pragma once

#include <list>
#include <unordered_map>

#include "Common/TNode.h"

class PatternExtractor {
private:
    TNode *ast;
    unordered_map<TNode *, string> &nodeToStmtNumMap;
    unordered_map<string, pair<string, TNode *>> assignPatternMap;

    void mapAssignPattern(TNode *node);
    void dfs(TNode *node);

public:
    PatternExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap);

    void extractRelationship();

    unordered_map<string, pair<string, TNode *>> getAssignPatternMap();
};
