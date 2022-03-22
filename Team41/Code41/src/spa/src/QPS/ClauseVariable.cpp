#include "ClauseVariable.h"

#include <utility>

ClauseVariable::ClauseVariable() {}

ClauseVariable::ClauseVariable(variable_type type, string label, QueryDeclaration::design_entity_type designEntityType)
    : label(std::move(label)), designEntityType(designEntityType), type(type) {}

ClauseVariable::ClauseVariable(variable_type type, string label, QueryDeclaration qd)
    : label(std::move(label)), designEntityType(qd.getType()), qd(std::move(qd)), type(type) {}

QueryDeclaration::design_entity_type ClauseVariable::getDesignEntityType() const {
    return designEntityType;
}

bool ClauseVariable::isIdentifier() const {
    return type == identifier;
}

bool ClauseVariable::isWildCard() const {
    return type == wildcard;
}

bool ClauseVariable::isSynonym() const {
    return type == synonym;
}

bool ClauseVariable::isInteger() const {
    return type == integer;
}

ClauseVariable::variable_type ClauseVariable::getType() const {
    return type;
}

string ClauseVariable::getLabel() const {
    return label;
}

QueryDeclaration ClauseVariable::getQueryDeclaration() const {
    return qd;
}
