#include "ClauseVariable.h"

#include <utility>

ClauseVariable::ClauseVariable() {}

ClauseVariable::ClauseVariable(variable_type type, string label, QueryDeclaration::design_entity_type designEntityType)
    : type(type), label(std::move(label)), designEntityType(designEntityType) {}

ClauseVariable::ClauseVariable(variable_type type, string label, QueryDeclaration qd)
    : type(type), label(std::move(label)), qd(std::move(qd)), designEntityType(qd.getType()) {}

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

bool ClauseVariable::equals(ClauseVariable other) const {
    if (type != other.getType())
        return false;
    if (label != other.getLabel())
        return false;
    if (!qd.equals(other.getQueryDeclaration()))
        return false;
    if (designEntityType != other.getDesignEntityType())
        return false;
    return true;
}

ClauseVariable ClauseVariable::convertWildCard() const {
    return {wildcard, label, designEntityType};
}
