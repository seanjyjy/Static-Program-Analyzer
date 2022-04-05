#pragma once

#include "Common/AstNode/TNode.h"

class TreeUtils {
public:
    // returns true if the structures, types and values of both trees are recursively equal.
    static bool isEqual(TNode *n1, TNode *n2);

    static string serialize(TNode *root);
};
