#include "Le.h"

Le::Le(RelFactor *lhs, RelFactor *rhs): RelExpr(TNodeType::le, lhs, rhs) {
}
