#include "ClauseVariable.h"

ClauseVariable::ClauseVariable(variable_type type, string &label)
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

bool ClauseVariable::isInteger() {
    return type == integer;
}

ClauseVariable::variable_type ClauseVariable::getType() {
    return type;
}

string &ClauseVariable::getLabel() {
    return label;
}
