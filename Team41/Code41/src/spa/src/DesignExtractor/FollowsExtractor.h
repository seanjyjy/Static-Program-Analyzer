#pragma once

#include <list>
#include <unordered_map>

#include "Base/StmtNumExtractor.h"
#include "Common/TNode.h"

class FollowsExtractor: public StmtNumExtractor {
private:
    unordered_map<string, list<string>> followsTMap; // mapping of statement to list of statements that followsT it

    /**
     * Records current statement to list of statements that followsT it into followsTMap.
     *
     * @param node current TNode of AST which is a statement
     * @param followsLst list of statement numbers that followsT current statement
     */
    void mapFollows(TNode *node, list<string> &followsLst);

    /**
     * Traverses through AST, building followsT list for each stmtlist and recording them.
     *
     * @param node current TNode being processed in tree traversal
     */
    void dfs(TNode *node);

public:
    FollowsExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap);

    /**
     * Records all statements' followsT relationship in followsTMap.
     */
    void extract() override;

    /**
     * For each mapping of statement S to list of statements L that followsT,
     * the first statement in L directly follows S.
     *
     * @return Map of statement number to list of statement numbers that followsT it
     */
    unordered_map<string, list<string>> getFollowsTMap();
};
