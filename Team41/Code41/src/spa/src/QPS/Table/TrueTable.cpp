#include "TrueTable.h"

TrueTable::TrueTable() = default;
TrueTable::~TrueTable() = default;

bool TrueTable::isEmpty() {
    return false;
}

Table* TrueTable::mergeJoin(Table* intermediatePQLTable) {
    return intermediatePQLTable;
}

Table::TableType TrueTable::getType() {
    return Table::TrueTable;
}
