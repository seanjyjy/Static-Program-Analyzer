#include "TreeUtils.h"
#include <algorithm>

bool TreeUtils::isEqual(TNode *n1, TNode *n2) {
    // base case: if nodes are different types or children are different sizes, not equal
    if (n1->getType() != n2->getType() || n1->getChildren().size() != n2->getChildren().size()) return false;

    // compare underlying token value (if any)
    bool isEq;
    Token *val1 = n1->getVal();
    Token *val2 = n2->getVal();
    if (val1 == nullptr && val2 == nullptr) {
        isEq = true;
    } else if (val1 != nullptr && val2 != nullptr){
        isEq = val1->getVal() == val2->getVal();
    } else {
        // one nullptr, other is not nullptr
        isEq = false;
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