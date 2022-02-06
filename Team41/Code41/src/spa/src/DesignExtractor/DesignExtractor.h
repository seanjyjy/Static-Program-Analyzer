#pragma once

#include "Common/TNode.h"
#include "PKB/PKB.h"
#include <unordered_map>

class DesignExtractor {
private:
    TNode *ast;
    PKB *pkb;
    unordered_map<TNode *, string> nodeToStmtNumMap;

    void registerEntity(TNode *node, int &stmtNum);
    void registerEntities(); // statement number, register entities
    void registerProcedures();

public:
    DesignExtractor(TNode *ast, PKB *pkb);

    void extractDesign();

    void printStmtNums(); // debugging purposes

};
