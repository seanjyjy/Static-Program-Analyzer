#pragma once

#include "CFGNode.h"

class CFGBuilder {
private:
    unordered_map<string, vector<string>> stmtNextMap; // mapping of statement number stmtNums it Next
    vector<string> firstStmts; // stmtNums that appear first in every procedure
    CFGNode *cfg = new CFGNode("0"); // root node of CFG
    unordered_map<string, CFGNode *> stmtNumToNodeMap; // mapping of statement number to CFGNode*

    // Compare 2 strings like they are numbers. 1 if str1 > str2, 0 if str1 == str2 and -1 if str1 < str2
    int stringNumComp(const string &str1, const string &str2);

    // sort vector in stmtNextMap is decreasing stmtNum order
    void revSortChildren();

    // build stmtNumToNodeMap keys + instantiate CFGNode based on stmtNextMap
    void instantiateMap();

    // add edge between CFGNodes. Always add all forward edges before adding backward edges
    void addEdge(CFGNode *parentCFGNode, CFGNode *childCFGNode, bool isForward);

    void buildCFG();

public:
    // Required: stmtNextMap must contain ALL stmtNum, including those who have nobody Next
    CFGBuilder(unordered_map<string, vector<string>> stmtNextMap, vector<string> firstStmts);

    void build();

    CFGNode *getCFG();

    unordered_map<string, CFGNode *> getStmtNumToNodeMap();
};
