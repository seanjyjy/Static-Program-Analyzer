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

WithVariable::withRefType WithVariable::getType() {
    return type;
}

WithVariable::attributeName WithVariable::getAttr() {
    return attr;
}

QueryDeclaration WithVariable::getSynonym() {
    return synonym;
}

string WithVariable::getIdent() {
    return ident;
}

int WithVariable::getInteger() {
    return integer;
}

string WithVariable::getIntegerAsString() {
    return to_string(integer);
}

