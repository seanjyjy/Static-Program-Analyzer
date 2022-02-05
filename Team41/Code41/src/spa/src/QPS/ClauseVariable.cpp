#include "ClauseVariable.h"

ClauseVariable::ClauseVariable(variableType type, string &label)
    : type(type), label(label) {}

bool ClauseVariable::isIdentifier() {
    return type == identifier;
}

bool ClauseVariable::isWildCard() {
    return type == wildcard;
}

bool ClauseVariable::isSynonym() {
    return type == synonym;
}
