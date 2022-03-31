#include "NoneEntities.h"

bool NoneEntities::isNone() {
    return true;
}

string NoneEntities::toString() {
    return "THIS IS IMPOSSIBLE!";
}

int NoneEntities::hashCode() {
    return -1; // shouldnt occur
}

NoneEntities::NoneEntities() : Entities(nullptr) {}

NoneEntities::~NoneEntities() {
    delete reader;
}