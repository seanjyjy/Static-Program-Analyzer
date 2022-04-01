#pragma once

#include "Common/TNode.h"

/**
 * Base abstract class for Extractors.
 */
class BaseExtractor {
protected:
    TNode *ast; // root node of AST

public:
    BaseExtractor(TNode *ast) : ast(ast) {}

    /**
     * Extracts entities/relationships/etc.
     */
    virtual bool extract() = 0;
};
