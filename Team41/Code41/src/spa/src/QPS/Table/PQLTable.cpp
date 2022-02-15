#include "PQLTable.h"

#include <algorithm>
#include <unordered_set>
#include <utility>
#include <stdexcept>

bool PQLTable::isEmpty() {
    return size() == 0;
}

Table* PQLTable::mergeJoin(Table* intermediatePQLTable) {

    if (intermediatePQLTable == nullptr) {
        throw runtime_error("PQLTable provided is null!");
    }

    if (intermediatePQLTable == TrueTable::getTable()) {
        return this;
    }

    Header commonHeader = getCommonHeader(this, intermediatePQLTable);
    Header combinedHeader = getCombinedHeader(this, intermediatePQLTable);
    Table* newTable = new PQLTable(combinedHeader);

    // Sort Tables
    this->sort(commonHeader);
    intermediatePQLTable->sort(commonHeader);

    // Merge Tables
    mergeTable(this, intermediatePQLTable, newTable, commonHeader);

    return newTable;
}

void PQLTable::mergeTable(Table *leftTable, Table *rightTable, Table *newTable, const Header& commonHeader) {
    auto leftTableRow = leftTable->getRows();
    auto rightTableRow = rightTable->getRows();

    // have to separate else have warning: object backing the pointer will be destroyed at the end of the full-expression
    auto leftTableIndex = leftTableRow.begin();
    auto leftTableEnd = leftTableRow.end();
    auto rightTableIndex = rightTableRow.begin();
    auto rightTableEnd = rightTableRow.end();

    while (leftTableIndex != leftTableEnd && rightTableIndex != rightTableEnd) {
        int result = compareRow(*leftTableIndex, *rightTableIndex, commonHeader);

        if (result == -1) {
            leftTableIndex++;
            continue;
        }

        if (result == 1) {
            rightTableIndex++;
            continue;
        }

        unordered_set<const Row*> leftRowsSet;
        unordered_set<const Row*> rightRowsSet;

        auto fixedLeftIndexElement = leftTableIndex;
        auto fixedRightIndexElement = rightTableIndex;

        while (leftTableIndex != leftTableEnd && isEqual(*fixedLeftIndexElement, *leftTableIndex, commonHeader)) {
            leftRowsSet.insert(*leftTableIndex);
            leftTableIndex++;
        }

        while (rightTableIndex != rightTableEnd && isEqual(*fixedRightIndexElement, *rightTableIndex, commonHeader)) {
            rightRowsSet.insert(*rightTableIndex);
            rightTableIndex++;
        }

        // We are ensured that the common headers have the same value due to compareRow check
        // Generate the cartesian product
        for (auto leftRow : leftRowsSet) {
            for (auto rightRow : rightRowsSet) {
                newTable->addRow(combineRow(leftRow, rightRow));
            }
        }
    }
}

bool PQLTable::isEqual(const Row* rowA, const Row* rowB, const Header& commonHeader) {
    for (auto& headerField: commonHeader) {
        if (rowA->getValueAtColumn(headerField) != rowB->getValueAtColumn(headerField)) {
            return false;
        }
    }

    return true;
}

const Row* PQLTable::combineRow(const Row* rowA, const Row* rowB) const {
    Row* row = new Row();

    for (const auto& columnValuePair : rowA->getRow()) {
        row->addEntry(columnValuePair.first, columnValuePair.second);
    }

    for (const auto& columnValuePair : rowB->getRow()) {
        row->addEntry(columnValuePair.first, columnValuePair.second);
    }

    return row;
}

void PQLTable::sort(const Header& commonHeader) {
    if (commonHeader.empty()) { return; }
    std::sort(this->rows.begin(), this->rows.end(), [commonHeader](const Row* left, const Row* right){
        for (const string& headerField : commonHeader) {
            return left->getValueAtColumn(headerField) < right->getValueAtColumn(headerField);
        }
        return false;
    });
}

Header PQLTable::getCommonHeader(Table *leftTable, Table *rightTable) {
    Header leftTableHeader = leftTable->getHeader();
    Header rightTableHeader = rightTable->getHeader();
    Header commonHeader;

    for (auto const& leftHeader : leftTableHeader) {
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

    for (auto const& leftHeader : leftTableHeader) {
        combinedHeader.insert(leftHeader);
    }

    for (auto const& rightHeader : rightTableHeader) {
        combinedHeader.insert(rightHeader);
    }

    return combinedHeader;
}

int PQLTable::compareRow(const Row* leftRowPtr, const Row* rightRowPtr, const Header& commonHeader) const {
    auto leftRow = leftRowPtr->getRow();
    auto rightRow = rightRowPtr->getRow();

    for (auto const& headerField : commonHeader) {
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

    for (const auto& headerField : columnHeader) {
        if (!row->hasColumn(headerField)) {
            return false;
        }
    }

    return true;
}

void PQLTable::addRow(const Row* row) {
    if (row == nullptr) {
        throw runtime_error("Row is null");
    }

    if (!checkRowMatchesHeader(row)) {
       throw runtime_error("Current row doesnt match the header requirements");
    }

    this->rows.push_back(row);
}

bool PQLTable::hasColumn(const string& column) {
    Header columnHeader = getHeader();
    return columnHeader.find(column) != columnHeader.end();
}

bool PQLTable::hasRow(const Row *row) {
    for (auto cRow : this->rows) {
        if (*cRow == *row) {
            return true;
        }
    }
    return false;
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
    for(auto r : rows) {
        delete r;
    }
}

unordered_set<string> PQLTable::getColumn(string columnName) {
    unordered_set<string> result;

    for (auto& row : this->getRows()) {
        result.insert(row->getValueAtColumn(columnName));
    }

    return result;
}

bool PQLTable::isBooleanTable() {
    return false;
}
