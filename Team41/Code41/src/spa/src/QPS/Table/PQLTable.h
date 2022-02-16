#pragma once

#include "Table.h"
#include "TrueTable.h"
#include "FalseTable.h"

using namespace std;

class PQLTable : public Table {
private:
    Header header;
    vector<const Row *> rows;

    Header getCommonHeader(Table* leftTable, Table* rightTable);
    Header getCombinedHeader(Table *leftTable, Table *rightTable);
    // Determine row ordering based on common header
    int compareRow(const Row* leftRowPtr, const Row* rightRowPtr, const Header& commonHeader) const;
    // merge current table with intermediate pql table to produce a new table
    void mergeTable(Table* leftTable, Table* rightTable, Table* newTable, const Header& commonHeader);
    // Sort the table based on common header
    void sort(const Header& commonHeader) override;
    // Determine row equality based on common header
    bool isEqual(const Row* rowA, const Row* rowB, const Header& commonHeader);
    const Row* combineRow(const Row* rowA, const Row* rowB) const;
    // Checks if each column of the row matches the header defined
    bool checkRowMatchesHeader(const Row* row) const;
public:
    explicit PQLTable(Header header);
    ~PQLTable();
    bool isEmpty() override;
    bool hasColumn(const string& column);
    // This is just used in testing so its runtime doesn't matter
    bool hasRow(const Row* row) override;
    Header getHeader() const override;
    size_t size() override;
    void addRow(const Row* row) override;
    // Checks if table is a boolean table
    bool isBooleanTable();

    // merge the two table based on sort-merge join algorithm
    Table* mergeJoin(Table* intermediatePQLTable) override;

    vector<const Row *> getRows() override;
    virtual unordered_set<string> getColumn(string columnName) override;
};

