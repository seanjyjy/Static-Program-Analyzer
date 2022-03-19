#include "UnaryOp.h"

#include "Common/TNode.h"
#include "Common/TNodeType.h"

#include <stdexcept>

UnaryOp::UnaryOp(TNodeType type, TNode *expr): TNode(type, nullptr, {expr}) {
    if (type != TNodeType::notOp) throw runtime_error("invalid unary op type");
}
