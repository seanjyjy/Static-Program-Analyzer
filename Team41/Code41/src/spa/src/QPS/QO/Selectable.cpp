#include "Selectable.h"

Selectable::Selectable(selectableType type, QueryDeclaration synonym, attributeName attr):
    type(type), attr(attr), synonym(synonym) {}

Selectable::attributeName Selectable::getAttr() const {
    return attr;
}

Selectable::selectableType Selectable::getType() const {
    return type;
}

QueryDeclaration Selectable::getSynonym() const {
    return synonym;
}

string Selectable::getSynonymName() const {
    return synonym.getSynonym();
}
