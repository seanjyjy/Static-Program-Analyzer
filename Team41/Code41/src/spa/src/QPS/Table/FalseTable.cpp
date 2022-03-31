#include "FalseTable.h"

FalseTable::FalseTable() = default;

FalseTable::~FalseTable() = default;

bool FalseTable::isEmpty() {
    return true;
}

Table *FalseTable::mergeJoin(Table *) {
    return this;
}

bool FalseTable::isFalseTable() {
    return true;
}