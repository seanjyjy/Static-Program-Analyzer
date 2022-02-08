#include "TreeUtils.h"
#include <algorithm>

bool TreeUtils::isEqual(TNode *n1, TNode *n2) {
    // base case: if children are different sizes, not equal
    if (n1->getChildren().size() != n2->getChildren().size()) return false;
    // otherwise first compare type and underlying token value (if any)
    bool isEq = true;
    if (n1->getVal() == nullptr && n2->getVal() == nullptr) {
        isEq = isEq && (n1->getType() == n2->getType());
    } else {
        isEq = isEq && (n1->getType() == n2->getType()) && (n1->getVal()->getVal() == n2->getVal()->getVal());
    }
    // then compare children equality
    for (int i = 0; i < n1->getChildren().size(); i++) {
        isEq = isEq && isEqual(n1->getChildren()[i], n2->getChildren()[i]);
    }
    return isEq;
}

string TreeUtils::serialize(TNode *root) {
    string ret;
    ret += root->toString();
    vector<TNode *> ch = root->getChildren();
    for (TNode *child: ch) {
        ret += "[" + serialize(child) + "]";
    }
    return ret;
}