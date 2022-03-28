#include "BooleanTable.h"

vector<const Row *> BooleanTable::getRows() {
    return {};
}

void BooleanTable::addRow(const Row *) {
    throw runtime_error("Not allow to use addRow on boolean table");
}

Header BooleanTable::getHeader() const {
    return {};
}

bool BooleanTable::hasRow(const Row *) {
    return false;
}

size_t BooleanTable::size() {
    return 0;
}

unordered_set<string> BooleanTable::getColumn(const string &) {
    return {};
}

void BooleanTable::sort(const vector<string> &) {}

unordered_set<string> BooleanTable::getColumns(const vector<string> &) {
    return {};
}
