#include "Entities.h"

Entities::Entities(EntitiesReader *reader) : reader(reader) {}

bool Entities::isStmt() {
    return false;
}

bool Entities::isRead() {
    return false;
}

bool Entities::isPrint() {
    return false;
}

bool Entities::isCall() {
    return false;
}

bool Entities::isWhile() {
    return false;
}

bool Entities::isIf() {
    return false;
}

bool Entities::isAssign() {
    return false;
}

bool Entities::isVariable() {
    return false;
}

bool Entities::isConstant() {
    return false;
}

bool Entities::isProcedure() {
    return false;
}

EntitiesReader *Entities::getReader() {
    return reader;
}

bool Entities::equals(Entities *other) {
    return other != nullptr && hashCode() == other->hashCode();
}

Entities::~Entities() = default;
