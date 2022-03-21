#include <iostream>
#include <queue>
#include <unordered_set>

#include "CFGUtils.h"

using namespace std;

bool compareCFGNodes(CFGNode *n1, CFGNode *n2) {
    return n1->getStmtNum() < n2->getStmtNum();
}

bool CFGUtils::isVecSortSame(vector<CFGNode *> &v1, vector<CFGNode *> &v2) {
    if (v1.size() != v2.size()) return false;
    if (v1.empty()) return true;
    sort(v1.begin(), v1.end(), compareCFGNodes);
    sort(v2.begin(), v2.end(), compareCFGNodes);
    for (int i = 0; i < v1.size(); ++i)
        if (v1[i]->getStmtNum() != v2[i]->getStmtNum())
            return false;
    return true;
}

bool CFGUtils::isEqual(CFGNode *n1, CFGNode *n2) {
    unordered_set<CFGNode *> seen;
    queue<pair<CFGNode *, CFGNode *>> bfsQ; // same nodes of both CFG
    bfsQ.push({n1, n2});
    seen.insert(n1); seen.insert(n2);
    while (!bfsQ.empty()) {
        tie(n1, n2) = bfsQ.front(); bfsQ.pop();
        if (n1->getStmtNum() != n2->getStmtNum()) return false;
        vector<CFGNode *> v1 = n1->getParent(), v2 = n2->getParent();
        if (!isVecSortSame(v1, v2)) return false;
        v1 = n1->getChildren(); v2 = n2->getChildren();
        if (!isVecSortSame(v1, v2)) return false;

        for (int i = 0; i < v1.size(); ++i) { // both children all contain same stmtNum
            auto it1 = seen.find(v1[i]), it2 = seen.find(v2[i]);
            if (it1 == seen.end() && it2 == seen.end()) {
                seen.insert(v1[i]); seen.insert(v2[i]);
                bfsQ.push({v1[i], v2[i]});
            } else if (it1 == seen.end() ^ it2 == seen.end()) { // one exists while other doesn't
                return false;
            }
        }
    }
    return true;
}

void CFGUtils::printCFG(CFGNode *node) {
    unordered_set<CFGNode *> seen;
    queue<CFGNode *> bfsQ;
    bfsQ.push(node);
    seen.insert(node);
    while (!bfsQ.empty()) {
        node = bfsQ.front(); bfsQ.pop();
        string stmtNum = node->getStmtNum();
        cout << stmtNum << ": ";
        for (CFGNode *child : node->getChildren()) {
            cout << child->getStmtNum() << ", ";
            if (seen.find(child) != seen.end()) continue;
            bfsQ.push(child);
            seen.insert(child);
        }
        cout << endl;
    }
}
