#include "ReadEntities.h"

bool ReadEntities::isRead() {
    return true;
}

EntitiesReader* ReadEntities::getReader() {
    return reader;
}

string ReadEntities::toString() {
    return "read";
}

int ReadEntities::hashCode() {
    return 6;
}

ReadEntities::ReadEntities() : Entities(new ReadReader()) {}

ReadEntities::~ReadEntities() {
    delete reader;
}