#include "ConstantEntities.h"

bool ConstantEntities::isConstant() {
    return true;
}

string ConstantEntities::toString() {
    return "constant";
}

int ConstantEntities::hashCode() {
    return 2;
}

ConstantEntities::ConstantEntities() : Entities(new ConstantReader()) {}

ConstantEntities::~ConstantEntities() {
    delete reader;
}
