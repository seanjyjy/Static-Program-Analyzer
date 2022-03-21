#pragma once

#include <unordered_map>

#include "BaseExtractor.h"

/**
 * Abstract class for extractors that require TNode to Stmt Num Mapping.
 */
class StmtNumExtractor: public BaseExtractor {
protected:
    const unordered_map<TNode *, string> &nodeToStmtNumMap; // mapping of TNode* to statement number

public:
    StmtNumExtractor(TNode *ast, unordered_map<TNode *, string> &nodeToStmtNumMap) :
            BaseExtractor(ast), nodeToStmtNumMap(nodeToStmtNumMap) {}
};
