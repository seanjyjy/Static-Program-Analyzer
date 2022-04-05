#pragma once

#include "TNode.h"
#include "TNodeType.h"
#include <stdexcept>

/**
 * Represents rel_factor -> var_name
							| const_value
							| expr
 */
class RelFactor : public TNode {
private:
    static TNodeType verify(TNodeType type);

    static Token *verify(TNodeType type, Token *val);

    static vector<TNode *> verify(TNodeType type, vector<TNode *> &children);

    RelFactor *parent = nullptr;
public:
    static RelFactor *dummy();

    RelFactor(TNodeType type, Token *val, vector<TNode *> children);

    void setRelParent(RelFactor *par);

    RelFactor *getRelParent();

    string toString() override;
};
