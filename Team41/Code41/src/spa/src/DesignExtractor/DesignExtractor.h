#pragma once

#include <unordered_map>
#include "Common/TNode.h"
#include "PKB/PKBManager.h"
#include "EntitiesExtractor.h"
#include "ModifiesExtractor.h"
#include "UsesExtractor.h"
#include "FollowsExtractor.h"
#include "ParentExtractor.h"
#include "PatternExtractor.h"

class DesignExtractor {
private:
    TNode *ast; // root node of AST
    PKBManager *pkb; // pointer to PKBManager to store all entities and relationships
    unordered_map<TNode *, string> nodeToStmtNumMap; // mapping of TNode* to statement number

    void extractEntities();
    void extractModifies();
    void extractUses();
    void extractFollows();
    void extractParent();
    void extractPattern();
public:
    DesignExtractor(TNode *ast, PKBManager *pkb);

    /**
     * Extracts all entities (procedures, statements, variables and constants) and relationships
     * and stores them in the PKBManager sequentially.
     * Supported relationships: Modifies, Uses, Follows(T) and Parent(T)
     */
    void extractDesign();
};
