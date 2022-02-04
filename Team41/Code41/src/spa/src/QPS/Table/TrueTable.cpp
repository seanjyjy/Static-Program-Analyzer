#include "TrueTable.h"

TrueTable::TrueTable() = default;

bool TrueTable::isEmpty() {
    return false;
}

Table* TrueTable::mergeJoin(Table* intermediatePQLTable) {
    return intermediatePQLTable;
}

vector<const Row *> TrueTable::getRows() {
    return {};
}

void TrueTable::addRow(const Row* row) {
    throw runtime_error("not allowed to add rows to true table");
}

Header TrueTable::getHeader() const {
    return {};
}

bool TrueTable::hasRow(const Row* row) {
    return false;
}

size_t TrueTable::size() {
    return 0;
}