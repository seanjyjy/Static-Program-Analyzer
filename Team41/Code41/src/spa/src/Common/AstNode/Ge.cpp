#include "Ge.h"

Ge::Ge(RelFactor *lhs, RelFactor *rhs) : RelExpr(TNodeType::ge, lhs, rhs) {
}
