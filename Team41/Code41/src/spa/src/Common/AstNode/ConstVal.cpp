#include "ConstVal.h"

ConstVal::ConstVal(Token *val): TNode(TNodeType::constValue, val, {}) {
}
