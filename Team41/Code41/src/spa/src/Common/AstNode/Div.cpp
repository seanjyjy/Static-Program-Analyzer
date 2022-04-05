#include "Div.h"

Div::Div(RelFactor *lhs, RelFactor *rhs) : RelFactor(TNodeType::div, nullptr, {lhs, rhs}) {}

string Div::toString() {
    return "/";
}

bool Div::isDiv() const {
    return true;
}
