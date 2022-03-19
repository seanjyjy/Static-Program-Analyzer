#include "Lt.h"

Lt::Lt(RelFactor *lhs, RelFactor *rhs) : RelExpr(TNodeType::lt, lhs, rhs) {
}
