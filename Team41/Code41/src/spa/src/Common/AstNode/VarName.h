#pragma once

#include "Common/AstNode/RelFactor.h"

class VarName : public RelFactor {
 public:
  VarName(Token *val);
};
