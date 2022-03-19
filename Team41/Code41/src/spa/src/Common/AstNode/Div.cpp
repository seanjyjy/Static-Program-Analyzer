#include "Div.h"

Div::Div(RelFactor *lhs, RelFactor *rhs): RelFactor(TNodeType::div, nullptr, {lhs,rhs}) {
}
