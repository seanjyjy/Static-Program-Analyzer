#pragma once

#include "CFGNode.h"

class CFGBuilder {
private:
    unordered_map<string, vector<string>> stmtNextMap; // mapping of statement number stmtNums it Next
    vector<string> firstStmts; // stmtNums that appear first in every procedure
    CFGNode *cfg = new CFGNode("0"); // root node of CFG
    unordered_map<string, CFGNode *> stmtNumToNodeMap; // mapping of statement number to CFGNode*


    // build stmtNumToNodeMap keys + instantiate CFGNode based on stmtNextMap
    void instantiateMap();

    // add edge between CFGNodes
    void addEdge(CFGNode *parentCFGNode, CFGNode *childCFGNode);

    void buildCFG();

public:
    // Required: stmtNextMap must contain ALL stmtNum, including those who have nobody Next
    CFGBuilder(unordered_map<string, vector<string>> stmtNextMap, vector<string> firstStmts);

    void build();

    CFGNode *getCFG();

    unordered_map<string, CFGNode *> getStmtNumToNodeMap();
};
