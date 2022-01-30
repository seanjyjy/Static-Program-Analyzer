#include "PQLTable.h"

#include <algorithm>
#include <unordered_set>
#include <utility>

bool PQLTable::isEmpty() {
    return size() == 0;
}

PQLTable* PQLTable::mergeJoin(PQLTable* intermediatePQLTable) {
    if (intermediatePQLTable == nullptr) {
        throw runtime_error("PQLTable provided is null!");
    }

    Header commonHeader = getCommonHeader(this, intermediatePQLTable);
    Header combinedHeader = getCombinedHeader(this, intermediatePQLTable);
    auto* newTable = new PQLTable(combinedHeader);

    // Sort Tables
    sort(this, commonHeader);
    sort(intermediatePQLTable, commonHeader);

    // Merge Tables
    mergeTable(this, intermediatePQLTable, newTable, commonHeader);
    delete intermediatePQLTable;

    return newTable;
}

void PQLTable::mergeTable(PQLTable *leftTable, PQLTable *rightTable, PQLTable *newTable, const Header& commonHeader) {
    auto leftTableIndex = leftTable->rows.begin();
    auto leftTableEnd = leftTable->rows.end();
    auto rightTableIndex = rightTable->rows.begin();
    auto rightTableEnd = rightTable->rows.end();


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

void PQLTable::sort(PQLTable* table, const Header& commonHeader) {
    std::sort(table->rows.begin(), table->rows.end(), [commonHeader](const Row* left, const Row* right){
        for (auto const& headerField : commonHeader) {
            return left->getValueAtColumn(headerField) <= right->getValueAtColumn(headerField);
        }
        return true;
    });
}

Header PQLTable::getCommonHeader(PQLTable *leftTable, PQLTable *rightTable) {
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

Header PQLTable::getCombinedHeader(PQLTable *leftTable, PQLTable *rightTable) {
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