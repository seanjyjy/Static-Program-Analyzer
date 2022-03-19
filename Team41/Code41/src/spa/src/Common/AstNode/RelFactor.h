#pragma once

#include "Common/TNode.h"
#include "Common/TNodeType.h"
#include <stdexcept>

class RelFactor: public TNode {
private:
    static TNodeType verify(TNodeType type);
    static Token* verify(TNodeType type, Token *val);
    static vector<TNode*> verify(TNodeType type, vector<TNode*> &children);

    RelFactor *parent = nullptr;
public:
    static RelFactor* dummy();
    RelFactor(TNodeType type, Token *val, vector<TNode*> children);
    void setRelParent(RelFactor *par);
    RelFactor *getRelParent();
};
