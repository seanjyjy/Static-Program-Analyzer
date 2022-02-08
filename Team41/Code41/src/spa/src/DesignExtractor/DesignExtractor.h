#pragma once

#include <unordered_map>
#include "Common/TNode.h"
#include "PKB/PKB.h"
#include "EntitiesExtractor.h"
#include "ModifiesExtractor.h"
#include "UsesExtractor.h"

class DesignExtractor {
private:
    TNode *ast;
    PKB *pkb;
    unordered_map<TNode *, string> nodeToStmtNumMap;

    void extractEntities();
    void extractModifies();
    void extractUses();

public:
    DesignExtractor(TNode *ast, PKB *pkb);

    void extractDesign();
};
