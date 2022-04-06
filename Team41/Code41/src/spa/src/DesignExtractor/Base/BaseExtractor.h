#pragma once

#include "Common/AstNode/TNode.h"

/**
 * Base abstract class for Extractors.
 */
class BaseExtractor {
protected:
    TNode *ast; // root node of AST
    const int condExpr = 0; // index of condExpr of if and while nodes
    const int whileStmtLst = 1; // index of stmtLst of while node
    const int ifStmtLstFirst = 1; // index of first stmtLst of if node
    const int ifStmtLstSecond = 2; // index of second stmtLst of if node

public:
    BaseExtractor(TNode *ast) : ast(ast) {}

    /**
     * Extracts entities/relationships/etc.
     */
    virtual bool extract() = 0;
};
