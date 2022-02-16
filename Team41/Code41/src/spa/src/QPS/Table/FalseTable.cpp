#include "FalseTable.h"
#include <stdexcept>

FalseTable::FalseTable() = default;
FalseTable::~FalseTable() {
    FalseTable::falseTable = nullptr;
};
Table* FalseTable::falseTable = nullptr;

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

unordered_set<string> FalseTable::getColumn(string columnName) {
    return unordered_set<string>();
}

Table* FalseTable::getTable() {
    if (FalseTable::falseTable == nullptr) {
        FalseTable::falseTable = new FalseTable();
    }
    return FalseTable::falseTable;
}

void FalseTable::sort(const Header &commonHeader) {}

bool FalseTable::isBooleanTable() {
    return true;
}
