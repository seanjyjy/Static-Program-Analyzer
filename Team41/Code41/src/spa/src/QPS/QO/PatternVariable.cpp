#include "PatternVariable.h"

bool PatternVariable::isWildcard() {
    return type == wildcard;
}

bool PatternVariable::isFullPattern() {
    return type == fullpattern;
}

bool PatternVariable::isSubPattern() {
    return type == subpattern;
}

TNode *PatternVariable::getMiniAST() {
    return miniAST;
}

string PatternVariable::getExpr() const {
    return expr;
}

PatternVariable::pattern_type PatternVariable::getType() const {
    return type;
}

bool PatternVariable::equals(PatternVariable other) const {
    if (type != other.getType())
        return false;
    return expr == other.getExpr();
}

PatternVariable::PatternVariable(pattern_type type, TNode *miniAST, string expr) : miniAST(miniAST), type(type),
                                                                                   expr(expr) {}

PatternVariable::PatternVariable(pattern_type type, TNode *miniAST) : miniAST(miniAST), type(type), expr("") {}

void PatternVariable::cleanAST() {
    delete miniAST;
    miniAST = nullptr;
}
