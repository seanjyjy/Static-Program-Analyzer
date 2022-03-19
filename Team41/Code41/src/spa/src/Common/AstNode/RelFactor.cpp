#include "RelFactor.h"

TNodeType RelFactor::verify(TNodeType type) {
    if (type != TNodeType::varName &&
        type != TNodeType::constValue &&
        type != TNodeType::plus &&
        type != TNodeType::minus &&
        type != TNodeType::times &&
        type != TNodeType::div &&
        type != TNodeType::mod &&
        type != TNodeType::dummy) { // dummy needed for building ast
        throw runtime_error("invalid node type for relfactor " + TNode::typeToString(type));
    }
    return type;
}

vector<TNode*> RelFactor::verify(TNodeType type, vector<TNode*> &children) {
    switch (type) {
        case TNodeType::plus:
        case TNodeType::minus:
        case TNodeType::times:
        case TNodeType::div:
        case TNodeType::mod:
            if (children.size() != 2) throw runtime_error("invalid number of children");
        default:
            break;
    }
    return children;
}

Token *RelFactor::verify(TNodeType type, Token *val) {
    switch (type) {
        case TNodeType::varName:
        case TNodeType::constValue:
            if (val == nullptr) throw runtime_error("token val should not be null");
        default:
            break;
    }
    return val;
}

RelFactor::RelFactor(TNodeType type, Token *val, vector<TNode *> children) : TNode(RelFactor::verify(type), RelFactor::verify(type, val), RelFactor::verify(type, children)) {
}

RelFactor *RelFactor::dummy() {
    return new RelFactor(TNodeType::dummy, nullptr, {});
}

void RelFactor::setRelParent(RelFactor *par) {
    parent = par;
    setParent(static_cast<TNode*>(par));
}

RelFactor *RelFactor::getRelParent() {
    return parent;
}
