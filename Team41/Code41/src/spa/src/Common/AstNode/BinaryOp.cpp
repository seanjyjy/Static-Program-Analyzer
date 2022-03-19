#include "BinaryOp.h"

#include <stdexcept>

BinaryOp::BinaryOp(TNodeType type, TNode *lhs, TNode *rhs): TNode(TNode(type, nullptr, {lhs, rhs})) {
    if (type != TNodeType::andOp &&
        type != TNodeType::orOp &&
        type != TNodeType::gt &&
        type != TNodeType::ge &&
        type != TNodeType::lt &&
        type != TNodeType::le &&
        type != TNodeType::eq &&
        type != TNodeType::ne &&
        type != TNodeType::plus &&
        type != TNodeType::minus &&
        type != TNodeType::times &&
        type != TNodeType::div &&
        type != TNodeType::mod) {
        throw runtime_error("invalid type for binary operator");
    }
}
