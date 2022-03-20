#include "ConstVal.h"

ConstVal::ConstVal(Token *val) : RelFactor(TNodeType::constValue, val, {}) {
}
