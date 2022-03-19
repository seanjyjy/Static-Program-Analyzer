#include "Not.h"

Not::Not(TNode *expr): UnaryOp(TNodeType::notOp, expr) {
}
