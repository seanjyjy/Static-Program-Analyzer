#include "And.h"

And::And(TNode *lhsCondExpr, TNode *rhsCondExpr): BinaryOp(TNodeType::andOp, lhsCondExpr, rhsCondExpr) {
}
