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

PatternVariable::PatternVariable(pattern_type type, TNode *miniAST) :
    type(type), miniAST(miniAST) {}