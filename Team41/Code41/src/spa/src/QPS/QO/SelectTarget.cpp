#include "SelectTarget.h"

SelectTarget::SelectTarget(targetType type) : type(type) {}

bool SelectTarget::isBoolean() const {
    return type == BOOLEAN;
}

bool SelectTarget::isTuple() const {
    return type == TUPLE;
}

void SelectTarget::addSelectable(Selectable newSelectable) {
    tuple.push_back(newSelectable);
}

void SelectTarget::setType(targetType targetType) {
    this->type = targetType;
}

vector<Selectable> SelectTarget::getSelectable() const {
    return tuple;
}
