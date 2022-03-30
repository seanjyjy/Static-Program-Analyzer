#include "VariableEntities.h"

bool VariableEntities::isVariable() {
    return true;
}

EntitiesReader* VariableEntities::getReader() {
    return reader;
}

string VariableEntities::toString() {
    return "variable";
}

int VariableEntities::hashCode() {
    return 8;
}

VariableEntities::VariableEntities() : Entities(new VariableReader()) {}

VariableEntities::~VariableEntities() {
    delete reader;
}
