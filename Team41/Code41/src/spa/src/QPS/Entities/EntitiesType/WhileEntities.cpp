#include "WhileEntities.h"

bool WhileEntities::isWhile() {
    return true;
}

string WhileEntities::toString() {
    return "while";
}

int WhileEntities::hashCode() {
    return 9;
}

WhileEntities::WhileEntities() : Entities(new WhileReader()) {}

WhileEntities::~WhileEntities() {
    delete reader;
}
