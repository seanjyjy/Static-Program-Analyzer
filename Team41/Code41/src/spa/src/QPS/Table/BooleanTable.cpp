#include "BooleanTable.h"

vector<const Row *> BooleanTable::getRows() {
    return {};
}

void BooleanTable::addRow(const Row *row) {
    throw runtime_error("Not allow to use addRow on boolean tabls");
}

Header BooleanTable::getHeader() const {
    return {};
}

bool BooleanTable::hasRow(const Row *row) {
    return false;
}

size_t BooleanTable::size() {
    return 0;
}

unordered_set<string> BooleanTable::getColumn(string columnName) {
    return {};
}

void BooleanTable::sort(vector<string> commonHeader) {}

unordered_set<string> BooleanTable::getColumns(vector<string> columnNames) {
    return {};
}
