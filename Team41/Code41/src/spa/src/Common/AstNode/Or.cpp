#include "Or.h"

#include "Common/TNode.h"
#include "Common/AstNode/BinaryOp.h"

Or::Or(TNode *lhsCondExpr, TNode *rhsCondExpr): BinaryOp(TNodeType::orOp, lhsCondExpr, rhsCondExpr) {
}
