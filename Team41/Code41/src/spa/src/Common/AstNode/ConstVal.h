#pragma once

#include "Common/AstNode/RelFactor.h"

class ConstVal : public RelFactor {
 public:
  ConstVal(Token *val);
};
