#include "Plus.h"

Plus::Plus(RelFactor *lhs, RelFactor *rhs) : RelFactor(TNodeType::plus, nullptr, {lhs, rhs}) {
}
