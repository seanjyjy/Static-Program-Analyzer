#include "SelectTarget.h"

SelectTarget::SelectTarget(targetType type) : type(type) {}

bool SelectTarget::isBoolean() {
    return type == BOOLEAN;
}

bool SelectTarget::isTuple() {
    return type == TUPLE;
}

void SelectTarget::addSelectable(Selectable newSelectable) {
    // todo: populate the vector<Selectable> tuple
    tuple.push_back(newSelectable);
}

void SelectTarget::setType(targetType type) {
    this->type = type;
}

vector<Selectable> SelectTarget::getSelectable() {
    return tuple;
}
