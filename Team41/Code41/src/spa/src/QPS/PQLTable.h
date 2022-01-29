#pragma once

#include <vector>
#include <string>
#include "Row.h"
#include <unordered_set>

using namespace std;

using Header = unordered_set<string>;
using Iterator = std::vector<const Row *>::iterator;

class PQLTable {
private:
    Header header;
    vector<const Row *> rows;

    Header getCommonHeader(PQLTable* leftTable, PQLTable* rightTable);
    Header getCombinedHeader(PQLTable *leftTable, PQLTable *rightTable);
    // Determine row ordering based on common header
    int compareRow(const Row* leftRowPtr, const Row* rightRowPtr, const Header& commonHeader) const;
    // merge current table with intermediate pql table to produce a new table
    void mergeTable(PQLTable* leftTable, PQLTable* rightTable, PQLTable* newTable, const Header& commonHeader);
    // Sort the table based on common header
    void sort(PQLTable* table, const Header& commonHeader);
    // Determine row equality based on common header
    bool isEqual(const Row* rowA, const Row* rowB, const Header& commonHeader);
    const Row* combineRow(const Row* rowA, const Row* rowB) const;
    // Checks if each column of the row matches the header defined
    bool checkRowMatchesHeader(const Row* row) const;
public:
    explicit PQLTable(Header header);
    ~PQLTable();
    bool isEmpty();
    bool hasColumn(const string& column);
    // This is just used in testing so its runtime doesn't matter
    bool hasRow(const Row* row);
    Header getHeader() const;
    size_t size();
    void addRow(const Row* row);

    // merge the two table based on sort-merge join algorithm
    PQLTable* mergeJoin(PQLTable* intermediatePQLTable);

    vector<const Row *> getRows();
};

