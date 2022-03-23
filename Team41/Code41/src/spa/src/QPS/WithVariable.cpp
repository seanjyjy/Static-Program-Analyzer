#include "WithVariable.h"

WithVariable::WithVariable(int integer): integer(integer) {
    type = INTEGER;
}

WithVariable::WithVariable(string ident): ident(ident) {
    type = IDENT;
    attr = NONE;
}

WithVariable::WithVariable(attributeName attr, QueryDeclaration syn):
                           attr(attr), synonym(syn) {
    type = ATTR_REF;
}

WithVariable::withRefType WithVariable::getType() const {
    return type;
}

WithVariable::attributeName WithVariable::getAttr() const {
    return attr;
}

QueryDeclaration WithVariable::getSynonym() const {
    return synonym;
}

string WithVariable::getIdent() const {
    return ident;
}

int WithVariable::getInteger() const {
    return integer;
}

string WithVariable::getIntegerAsString() const {
    return to_string(integer);
}

bool WithVariable::isIdentifier() const {
    return type == IDENT;
}

bool WithVariable::isInteger() const {
    return type == INTEGER;
}

bool WithVariable::isAttrRef() const {
    return type == ATTR_REF;
}

bool WithVariable::equals(WithVariable other) const {
    if (type != other.getType())
        return false;

    if (isAttrRef() && attr != other.getAttr())
        return false;

    if (isAttrRef() && !synonym.equals(other.getSynonym()))
        return false;

    if (isInteger() && integer != other.getInteger())
        return false;

    if (isIdentifier() && ident != other.getIdent())
        return false;

    return true;
}

string WithVariable::toString() const {
    string out = "";
    if (isInteger())
        out += "int: " + getIntegerAsString();
    if (isIdentifier())
        out += "ident: " + getIdent();
    if (isAttrRef()) {
        out += synonym.getSynonym();
        if (attr == PROC_NAME)
            out += ".procName";
        else if (attr == VAR_NAME)
            out += ".varName";
        else if (attr == VALUE)
            out += ".value";
        else if (attr == STMT_NUM)
            out += ".stmt#";
    }

    return out;
}
