#include "PQLTable.h"

#include <algorithm>
#include <unordered_set>
#include <utility>
#include <stdexcept>

bool PQLTable::isEmpty() {
    return size() == 0;
}

Table *PQLTable::mergeJoin(Table *intermediatePQLTable) {
    if (intermediatePQLTable == nullptr) {
        throw runtime_error("PQLTable provided is null!");
    }

    if (intermediatePQLTable->getType() == Table::TrueTable) {
        return this;
    }

    Header commonHeader = getCommonHeader(this, intermediatePQLTable);
    Header combinedHeader = getCombinedHeader(this, intermediatePQLTable);
    Table *newTable = new PQLTable(combinedHeader);

    vector<string> orderedCommonHeader;
    orderedCommonHeader.insert(orderedCommonHeader.end(), commonHeader.begin(), commonHeader.end());
    std::sort(orderedCommonHeader.begin(), orderedCommonHeader.end());

    // Sort Tables
    this->sort(orderedCommonHeader);
    intermediatePQLTable->sort(orderedCommonHeader);

    // Merge Tables
    mergeTable(this, intermediatePQLTable, newTable, orderedCommonHeader);

    return newTable;
}

void PQLTable::mergeTable(Table *leftTable, Table *rightTable, Table *newTable, const vector<string> &commonHeader) {
    vector<const Row *> leftTableRow = leftTable->getRows();
    vector<const Row *> rightTableRow = rightTable->getRows();

    // have to separate else have warning: object backing the pointer will be destroyed at the end of the full-expression
    auto leftTableIndex = leftTableRow.begin();
    auto leftTableEnd = leftTableRow.end();
    auto rightTableIndex = rightTableRow.begin();
    auto rightTableEnd = rightTableRow.end();

    while (leftTableIndex < leftTableEnd && rightTableIndex < rightTableEnd) {
        int result = compareRow(*leftTableIndex, *rightTableIndex, commonHeader);
        if (result == -1) {
            leftTableIndex++;
            continue;
        }

        if (result == 1) {
            rightTableIndex++;
            continue;
        }

        unordered_set<const Row *> leftRowsSet;
        unordered_set<const Row *> rightRowsSet;

        auto leftIndexElement = leftTableIndex;
        auto rightIndexElement = rightTableIndex;

        while (leftTableIndex < leftTableEnd && isEqual(*leftIndexElement, *leftTableIndex, commonHeader)) {
            leftRowsSet.insert(*leftTableIndex);
            leftTableIndex++;
        }

        while (rightTableIndex < rightTableEnd && isEqual(*rightIndexElement, *rightTableIndex, commonHeader)) {
            rightRowsSet.insert(*rightTableIndex);
            rightTableIndex++;
        }

        // We are ensured that the common headers have the same value due to compareRow check
        // Generate the cartesian product
        for (auto leftRow: leftRowsSet) {
            for (auto rightRow: rightRowsSet) {
                newTable->addRow(combineRow(leftRow, rightRow));
            }
        }
    }
}

bool PQLTable::isEqual(const Row *rowA, const Row *rowB, const vector<string> &orderedCommonHeader) {
    return all_of(orderedCommonHeader.begin(), orderedCommonHeader.end(), [&rowA, &rowB](auto &headerField) {
        return rowA->getValueAtColumn(headerField) == rowB->getValueAtColumn(headerField);
    });
}

const Row *PQLTable::combineRow(const Row *rowA, const Row *rowB) const {
    Row *row = new Row();

    for (auto &[column, value]: rowA->getRow()) {
        row->addEntry(column, value);
    }

    for (auto &[column, value]: rowB->getRow()) {
        row->addEntry(column, value);
    }

    return row;
}

void PQLTable::sort(const vector<string> &orderedCommonHeader) {
    if (orderedCommonHeader.empty()) return;
    std::sort(this->rows.begin(), this->rows.end(), [&orderedCommonHeader](const Row *left, const Row *right) {
        for (const string &headerField: orderedCommonHeader) {
            string leftVal = left->getValueAtColumn(headerField);
            string rightVal = right->getValueAtColumn(headerField);
            if (leftVal != rightVal) {
                return leftVal < rightVal;
            }
        }
        return false;
    });
}

Header PQLTable::getCommonHeader(Table *leftTable, Table *rightTable) {
    Header leftTableHeader = leftTable->getHeader();
    Header rightTableHeader = rightTable->getHeader();
    Header commonHeader;

    for (auto const &leftHeader: leftTableHeader) {
        if (rightTableHeader.find(leftHeader) != rightTableHeader.end()) {
            commonHeader.insert(leftHeader);
        }
    }

    return commonHeader;
}

Header PQLTable::getCombinedHeader(Table *leftTable, Table *rightTable) {
    Header leftTableHeader = leftTable->getHeader();
    Header rightTableHeader = rightTable->getHeader();
    Header combinedHeader;

    for (auto const &leftHeader: leftTableHeader) {
        combinedHeader.insert(leftHeader);
    }

    for (auto const &rightHeader: rightTableHeader) {
        combinedHeader.insert(rightHeader);
    }

    return combinedHeader;
}

int
PQLTable::compareRow(const Row *leftRowPtr, const Row *rightRowPtr, const vector<string> &orderedCommonHeader) const {
    auto leftRow = leftRowPtr->getRow();
    auto rightRow = rightRowPtr->getRow();

    for (auto const &headerField: orderedCommonHeader) {
        if (leftRow.at(headerField) < rightRow.at(headerField)) {
            return -1;
        }

        if (leftRow.at(headerField) > rightRow.at(headerField)) {
            return 1;
        }
    }

    return 0;
}

bool PQLTable::checkRowMatchesHeader(const Row *row) const {
    Header columnHeader = this->getHeader();
    if (row->size() != columnHeader.size()) {
        return false;
    }

    return all_of(columnHeader.begin(), columnHeader.end(), [&row](auto &headerField) {
        return row->hasColumn(headerField);
    });
}

void PQLTable::addRow(const Row *row) {
    if (row == nullptr) {
        throw runtime_error("Row is null");
    }

    if (!checkRowMatchesHeader(row)) {
        throw runtime_error("Current row doesnt match the header requirements");
    }

    this->rows.push_back(row);
}

bool PQLTable::hasColumn(const string &column) {
    Header columnHeader = getHeader();
    return columnHeader.find(column) != columnHeader.end();
}

bool PQLTable::hasRow(const Row *row) {
    return any_of(rows.begin(), rows.end(), [&row](const Row *cRow) {
        return *cRow == *row;
    });
}

Header PQLTable::getHeader() const {
    return this->header;
}

size_t PQLTable::size() {
    return this->rows.size();
}

// Constructor
PQLTable::PQLTable(Header header) {
    this->header = std::move(header);
}

vector<const Row *> PQLTable::getRows() {
    return this->rows;
}

PQLTable::~PQLTable() {
    for (auto r: rows) {
        delete r;
    }
}

unordered_set<string> PQLTable::getColumn(const string &columnName) {
    unordered_set<string> results;

    for (auto &row: this->getRows()) {
        results.insert(row->getValueAtColumn(columnName));
    }

    return results;
}

Table::TableType PQLTable::getType() {
    return Table::PQLTable;
}

unordered_set<string> PQLTable::getColumns(const vector<string> &columnNames) {
    unordered_set<string> results;

    for (auto &row: this->getRows()) {
        string result;
        for (auto &columnName: columnNames) {
            result.append(row->getValueAtColumn(columnName)).append(" ");
        }
        if (!result.empty())
            results.insert(result.substr(0, result.size() - 1));
    }

    return results;
}
