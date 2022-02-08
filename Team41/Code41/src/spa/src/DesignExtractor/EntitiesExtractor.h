#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Common/TNode.h"

class EntitiesExtractor {
private:
    TNode *ast;
    unordered_map<TNode *, string> nodeToStmtNumMap;
    unordered_set<string> procSet;
    unordered_set<string> varSet;
    unordered_set<string> constSet;

    void findProcedures();
    void recordEntity(TNode *node, int &stmtNum);
    void findEntities(); // statement number, varName, const

public:
    EntitiesExtractor(TNode *ast);

    void extractEntities();

    unordered_map<TNode *, string> getNodeToStmtNumMap();
    unordered_set<string> getProcSet();
    unordered_set<string> getVarSet();
    unordered_set<string> getConstSet();
};
