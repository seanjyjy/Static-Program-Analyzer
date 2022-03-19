#include "VarName.h"

VarName::VarName(Token *val): TNode(TNodeType::varName, val, {}) {
}
