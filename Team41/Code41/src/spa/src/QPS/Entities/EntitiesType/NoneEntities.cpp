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

NoneEntities* NoneEntities::getInstance()  {
    // singleton instance in non-heap memory for automatic cleanUp
    static NoneEntities instance;
    return &instance;
}
