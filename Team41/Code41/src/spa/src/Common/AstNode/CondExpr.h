#pragma once

#include "Common/TNode.h"
#include "Common/TNodeType.h"
#include <stdexcept>
#include <utility>

using namespace std;

class CondExpr : public TNode {
 private:
  static TNodeType verify(TNodeType type);

 public:
  CondExpr(TNodeType type, vector<TNode *> children);
};
