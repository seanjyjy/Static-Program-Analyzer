#include "ClauseVariable.h"

#include <utility>

ClauseVariable::ClauseVariable(variable_type type, string label, QueryDeclaration::design_entity_type designEntityType)
    : type(type), label(std::move(label)), designEntityType(designEntityType) {}

QueryDeclaration::design_entity_type ClauseVariable::getDesignEntityType() {
    return designEntityType;
}

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

string ClauseVariable::getLabel() {
    return label;
}
