#include "Ne.h"

Ne::Ne(RelFactor *lhs, RelFactor *rhs) : RelExpr(TNodeType::ne, lhs, rhs) {
}
