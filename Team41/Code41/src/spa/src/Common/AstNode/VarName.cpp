#include "VarName.h"

VarName::VarName(Token *val): RelFactor(TNodeType::varName, val, {}) {
}
