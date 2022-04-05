#include <utility>
#include <stack>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include "SimpleParser/Token.h"
#include "TNode.h"

using namespace std;

TNode::TNode(TNodeType type, Token *val, vector<TNode *> children) : type(type), val(val), children(move(children)),
                                                                     parent(nullptr) {}

TNode::~TNode() {
    delete val;
    for (TNode *ch: children) {
        delete ch;
    }
}

void TNode::addChild(TNode *child) {
    children.push_back(child);
}

TNode *TNode::getParent() {
    return parent;
}

const vector<TNode *> &TNode::getChildren() {
    return children;
}

Token *TNode::getVal() {
    return val;
}

TNodeType TNode::getType() {
    return type;
}

string TNode::getTokenVal() {
    return val->getVal();
}

void TNode::setParent(TNode *par) {
    parent = par;
}

void TNode::setLeftChild(TNode *child) {
    if (children.empty()) throw runtime_error("children should not be empty");
    delete children[0];
    children[0] = child;
}

void TNode::setChildren(vector<TNode *> ch) {
    children = move(ch);
}

// dfs while setting parent pointers for all nodes
void TNode::setAllParents() {
    if (children.empty()) return;
    for (TNode *child: children) {
        child->setParent(this);
        child->setAllParents();
    }
}

string TNode::toString() {
    throw runtime_error("toString not implemented for TNode");
}

bool TNode::isProgram() const {
    return false;
}

bool TNode::isProcedure() const {
    return false;
}

bool TNode::isStmtLst() const {
    return false;
}

bool TNode::isRead() const {
    return false;
}

bool TNode::isPrint() const {
    return false;
}

bool TNode::isCall() const {
    return false;
}

bool TNode::isWhile() const {
    return false;
}

bool TNode::isIf() const {
    return false;
}

bool TNode::isAssign() const {
    return false;
}

bool TNode::isStmt() const {
    return false;
}

bool TNode::isNot() const {
    return false;
}

bool TNode::isAnd() const {
    return false;
}

bool TNode::isOr() const {
    return false;
}

bool TNode::isGt() const {
    return false;
}

bool TNode::isGe() const {
    return false;
}

bool TNode::isLt() const {
    return false;
}

bool TNode::isLe() const {
    return false;
}

bool TNode::isEq() const {
    return false;
}

bool TNode::isNe() const {
    return false;
}

bool TNode::isPlus() const {
    return false;
}

bool TNode::isMinus() const {
    return false;
}

bool TNode::isTimes() const {
    return false;
}

bool TNode::isDiv() const {
    return false;
}

bool TNode::isMod() const {
    return false;
}

bool TNode::isVarName() const {
    return false;
}

bool TNode::isProcName() const {
    return false;
}

bool TNode::isConstVal() const {
    return false;
}
