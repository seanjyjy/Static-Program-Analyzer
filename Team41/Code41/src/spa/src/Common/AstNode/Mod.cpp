#include "Mod.h"

Mod::Mod(RelFactor *lhs, RelFactor *rhs): RelFactor(TNodeType::mod, nullptr, {lhs, rhs}) {
}
