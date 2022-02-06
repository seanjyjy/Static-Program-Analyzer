#include "TrueTable.h"

TrueTable::TrueTable() = default;
Table* TrueTable::trueTable = nullptr;

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

unordered_set<string> TrueTable::getColumn(string columnName) {
    return unordered_set<string>();
}

Table* TrueTable::getTable() {
    if (TrueTable::trueTable == nullptr) {
        TrueTable::trueTable = new TrueTable();
    }
    return TrueTable::trueTable;
}
