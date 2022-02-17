#pragma once

#include "Table.h"
#include "TrueTable.h"
#include "FalseTable.h"

using namespace std;

class PQLTable : public Table {
private:
    Header header;
    vector<const Row *> rows;

    /**
     * Retrieves the common header between the two tables.
     *
     * @param leftTable Table to be merged.
     * @param rightTable Table to be merged.
     * @return A common header between the two tables.
     */
    Header getCommonHeader(Table* leftTable, Table* rightTable);

    /**
     * Combines the header between the two tables. If there are duplicate headers, it will be considered as the same.
     *
     * @param leftTable Table to be merged.
     * @param rightTable Table to be merged.
     * @return A combined header between the two tables.
     */
    Header getCombinedHeader(Table *leftTable, Table *rightTable);

    /**
     * Compares the order priority of the two rows based on the common header.
     *
     * @param leftRowPtr Row to be compared.
     * @param rightRowPtr Row to be compared.
     * @param commonHeader The common header that provides the column for ordering.
     * @return 0 if both rows are of the same order, -1 if leftRowPtr has a higher priority and 1 if rightRowPtr has
     * a lower priority.
     */
    int compareRow(const Row* leftRowPtr, const Row* rightRowPtr, const Header& commonHeader) const;

    /**
     * Merge both table based on the common header to form the resultant cross product table.
     *
     * @param leftTable Table to be merged.
     * @param rightTable Table to be merged.
     * @param newTable A new table to be populated with the result of the merging.
     * @param commonHeader The common header that provides the column for ordering.
     */
    void mergeTable(Table* leftTable, Table* rightTable, Table* newTable, const Header& commonHeader);

    /**
     * Sorts the table based on the common header fields.
     *
     * @param commonHeader The common header that provides the column for sorting order.
     */
    void sort(const Header& commonHeader) override;

    /**
     * Determines if two rows are equal based on the common header fields.
     *
     * @param rowA Row to be compared.
     * @param rowB Row to be compared.
     * @param commonHeader The common header that provides the column for equality check
     * @return True if both rows are considered equal based on common header, else false.
     */
    bool isEqual(const Row* rowA, const Row* rowB, const Header& commonHeader);

    /**
     * Combines the two rows together.
     *
     * @param rowA Row to be combined.
     * @param rowB Row to be combined.
     * @return A row that is the result of combining two rows.
     */
    const Row* combineRow(const Row* rowA, const Row* rowB) const;

    /**
     * Checks if the column of the row provided matches the header defined in the table.
     *
     * @param row Row to be checked.
     * @return True if the row provided matches the header defined in the table else false.
     */
    bool checkRowMatchesHeader(const Row* row) const;
public:
    explicit PQLTable(Header header);
    ~PQLTable();

    /**
     * @see QPS/Table/Table.h
     */
    bool isEmpty() override;

    /**
     * Checks if the current table contains a particular column.
     *
     * @param column The name of the column.
     * @return True if table contains this column else false.
     */
    bool hasColumn(const string& column);

    /**
     * @see QPS/Table/Table.h
     */
    bool hasRow(const Row* row) override;

    /**
     * @see QPS/Table/Table.h
     */
    Header getHeader() const override;

    /**
     * @see QPS/Table/Table.h
     */
    size_t size() override;

    /**
     * @see QPS/Table/Table.h
     */
    void addRow(const Row* row) override;

    TableType getType() override;

    /**
     * @see QPS/Table/Table.h
     */
    Table* mergeJoin(Table* intermediatePQLTable) override;

    /**
     * @see QPS/Table/Table.h
     */
    vector<const Row *> getRows() override;

    /**
     * @see QPS/Table/Table.h
     */
    virtual unordered_set<string> getColumn(string columnName) override;
};

