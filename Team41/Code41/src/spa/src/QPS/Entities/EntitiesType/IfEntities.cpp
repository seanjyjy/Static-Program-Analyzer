#include "IfEntities.h"

bool IfEntities::isIf() {
    return true;
}

string IfEntities::toString() {
    return "if";
}

int IfEntities::hashCode() {
    return 2;
}

IfEntities::IfEntities() : Entities(new IfReader()) {}

IfEntities::~IfEntities() {
    delete reader;
}