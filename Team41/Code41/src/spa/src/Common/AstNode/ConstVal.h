#pragma once

#include "Common/TNode.h"

class ConstVal: public TNode {
public:
    ConstVal(Token *val);
};
