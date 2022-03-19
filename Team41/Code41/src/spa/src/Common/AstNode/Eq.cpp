#include "Eq.h"

Eq::Eq(RelFactor *lhs, RelFactor *rhs): RelExpr(TNodeType::eq, lhs, rhs) {
}
