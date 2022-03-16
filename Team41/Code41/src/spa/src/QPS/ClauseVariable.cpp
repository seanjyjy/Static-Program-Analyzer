#include "ClauseVariable.h"

#include <utility>

ClauseVariable::ClauseVariable(variable_type type, string label, QueryDeclaration::design_entity_type designEntityType)
    : type(type), label(std::move(label)), designEntityType(designEntityType) {}

QueryDeclaration::design_entity_type ClauseVariable::getDesignEntityType() const {
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

ClauseVariable::variable_type ClauseVariable::getType() const {
    return type;
}

string ClauseVariable::getLabel() const {
    return label;
}
