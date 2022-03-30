#include "IfEntities.h"

bool IfEntities::isIf() {
    return true;
}

EntitiesReader *IfEntities::getReader() {
    return reader;
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