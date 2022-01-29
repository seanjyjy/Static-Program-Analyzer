#pragma once

#include <utility>

#include "Token.h"
#include "Common/TNodeType.h"
#include "SimpleParser/Token.h"

using namespace std;

class TNode {
private:
    TNodeType type;
    Token val;
    vector<TNode*> children;
    TNode *parent;
public:
    TNode(TNodeType type, Token val);
};