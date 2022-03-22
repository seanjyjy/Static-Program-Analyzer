#include "CondExpr.h"

TNodeType CondExpr::verify(TNodeType type) {
  if (type != TNodeType::notOp &&
	  type != TNodeType::andOp &&
	  type != TNodeType::orOp &&
	  type != TNodeType::gt &&
	  type != TNodeType::ge &&
	  type != TNodeType::lt &&
	  type != TNodeType::le &&
	  type != TNodeType::eq &&
	  type != TNodeType::ne) {
	throw runtime_error("invalid node type for condexpr");
  }
  return type;
}

CondExpr::CondExpr(TNodeType type, vector<TNode *> children) : TNode(CondExpr::verify(type), nullptr, move(children)) {
}
