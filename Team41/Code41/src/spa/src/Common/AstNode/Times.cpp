#include "Times.h"

Times::Times(RelFactor *lhs, RelFactor *rhs): RelFactor(TNodeType::times, nullptr, {lhs,rhs}) {
}
