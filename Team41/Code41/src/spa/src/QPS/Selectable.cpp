#include "Selectable.h"

Selectable::Selectable(selectableType type, QueryDeclaration synonym, attributeName attr):
    type(type), attr(attr), synonym(synonym) {}

Selectable::attributeName Selectable::getAttr() {
    return attr;
}

Selectable::selectableType Selectable::getType() {
    return type;
}

QueryDeclaration Selectable::getSynonym() {
    return synonym;
}
