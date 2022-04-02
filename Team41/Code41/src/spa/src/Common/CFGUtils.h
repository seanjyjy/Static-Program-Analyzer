#pragma once

#include <vector>

#include "CFGNode.h"

class CFGUtils {
private:
    static bool isVecSortSame(vector<CFGNode *> &v1, vector<CFGNode *> &v2);

public:
    // returns true if both graphs are equal (nodes and children and parent)
    static bool isEqual(CFGNode *n1, CFGNode *n2);

    // calls delete on each CFGNode pointer in the map
    static void deleteCFG(const unordered_map<string, CFGNode *> &stmtNumToNodeMap);

    // print next relations for troubleshooting
    static void printCFG(CFGNode *node);
};
