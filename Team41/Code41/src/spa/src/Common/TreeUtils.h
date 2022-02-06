#pragma once

#include "TNode.h"

class TreeUtils {
public:
    static bool isEqual(TNode *n1, TNode *n2);
    static string serialize(TNode *root);
    static string serializeWithSort(TNode *root);
};
