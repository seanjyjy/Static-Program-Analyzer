#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

#include "Common/TNode.h"
#include "PKB/PKBManager.h"
#include "CallsExtractor.h"
#include "EntitiesExtractor.h"
#include "ModifiesExtractor.h"
#include "UsesExtractor.h"
#include "FollowsExtractor.h"
#include "ParentExtractor.h"
#include "PatternExtractor.h"
#include "CFGExtractor.h"

class DesignExtractor {
private:
    TNode *ast; // root node of AST
    PKBManager *pkb; // pointer to PKBManager to store all entities and relationships
    unordered_map<TNode *, string> nodeToStmtNumMap; // mapping of TNode* to statement number
    unordered_set<string> procSet; // set of procedure names
    unordered_map<string, unordered_set<string>> callsMap; // mapping of proc to list of proc it directly calls
    list<string> procCallOrder; // list of procedures in reversed toposort order of calls graph

    bool extractEntities();

    bool extractCalls();

    bool extractModifies();

    bool extractUses();

    bool extractFollows();

    bool extractParent();

    bool extractPattern();

    bool extractCFG();

public:
    DesignExtractor(TNode *ast, PKBManager *pkb);

    /**
     * Extracts all entities (procedures, statements, variables and constants) and relationships
     * and stores them in the PKBManager sequentially.
     *
     * @return True if extractions were successful, else False
     */
    bool extractDesign();
};
