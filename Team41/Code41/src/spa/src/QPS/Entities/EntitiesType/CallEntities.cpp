#include "CallEntities.h"

bool CallEntities::isCall() {
    return true;
}

EntitiesReader *CallEntities::getReader() {
    return reader;
}

string CallEntities::toString() {
    return "call";
}

int CallEntities::hashCode() {
    return 1;
}

CallEntities::CallEntities() : Entities(new CallReader()) {}

CallEntities::~CallEntities() {
    delete reader;
}