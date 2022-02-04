#include "FalseTable.h"

FalseTable::FalseTable() = default;

bool FalseTable::isEmpty() {
    return true;
}

Table* FalseTable::mergeJoin(Table* intermediatePQLTable) {
    return this;
}

vector<const Row *> FalseTable::getRows() {
    return {};
}

void FalseTable::addRow(const Row* row) {
    throw runtime_error("not allowed to add rows to false table");
}

Header FalseTable::getHeader() const {
    return {};
}

bool FalseTable::hasRow(const Row* row) {
    return false;
}

size_t FalseTable::size() {
    return 0;
}