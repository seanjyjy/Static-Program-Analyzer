#pragma once

#include <list>
#include <unordered_map>

#include "Base/StmtNumExtractor.h"
#include "Common/TNode.h"

class ParentExtractor: public StmtNumExtractor {
private:
    unordered_map<string, list<string>> parentMap; // mapping of statement to list of statements that it is parent of
    unordered_map<string, list<string>> parentTMap;// mapping of statement to list of statements that it is parentT of

    /**
     * Records current statement to list of statements it is parent of by considering its children.
     *
     * @param node current TNode of AST which is a statement
     */
    void mapParent(TNode *node);

    /**
     * Records current statement to list of statements it is parentT of.
     *
     * @param node current TNode of AST which is a statement
     * @param parentLst list of statements which current statement is parentT of
     */
    void mapParentT(TNode *node, list<string> &parentLst);

    /**
     * Traverses through AST, building parentT list upwards and recording them for statements.
     *
     * @param node current TNode being processed in tree traversal
     * @param parentLst list containing statements, which current node is parent of
     */
    void dfs(TNode *node, list<string> &parentLst);

public:
    ParentExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap);

    /**
     * Records all parent and parentT relationship in parentMap and parentTMap.
     */
    void extract() override;

    unordered_map<string, list<string>> getParentMap();
    unordered_map<string, list<string>> getParentTMap();
};

