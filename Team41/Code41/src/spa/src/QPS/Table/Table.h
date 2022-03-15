#pragma once

#include <vector>
#include <string>
#include <unordered_set>

#include "Row.h"

using namespace std;

using Header = unordered_set<string>;
using Iterator = std::vector<const Row *>::iterator;

class Table {
public:
    enum TableType {
        TrueTable, FalseTable, PQLTable
    };
    /**
     * Checks if a table is empty.
     *
     * @return True if table is empty else false.
     */
    virtual bool isEmpty() = 0;

    /**
     * Merged two table together based on sort-merge join algorithm on common headers.
     *
     * @param intermediatePQLTable The other table to join with the current table.
     * @return A combined table based on the sort-merge join algorithm on common headers.
     */
    virtual Table* mergeJoin(Table* intermediatePQLTable) = 0;

    /**
     * Retrieves a vector of rows in this table.
     *
     * @return a vector of rows in this table.
     */
    virtual vector<const Row *> getRows() = 0;

    /**
     * Adds a row to this table.
     *
     * @param row Row to be added to the table.
     */
    virtual void addRow(const Row* row) = 0;

    /**
     * Retrieves the header of the table.
     *
     * @return the header of the table.
     */
    virtual Header getHeader() const = 0;

    /**
     * Checks if the table contains a certain row.
     *
     * @param row The row to be checked.
     * @return True if the table contains this row, else false.
     */
    virtual bool hasRow(const Row* row) = 0;

    /**
     * Retrieves the size of this table based on the number of rows.
     *
     * @return the size of this table.
     */
    virtual size_t size() = 0;

    /**
     * Retrieves a set of values from this table based on the column.
     *
     * @param columnName The name of the column to retrieve from.
     * @return A set of values from this table based on the column.
     */
    virtual unordered_set<string> getColumn(string columnName) = 0;

    /**
     * Retrieves a set of values from this table based on the column.
     *
     * @param columnName The names of the columns to retrieve from.
     * @return A set of values from this table based on the columns.
     */
    virtual unordered_set<string> getColumns(vector<string> columnNames) = 0;

    /**
     * Sort the current table based on the common header provided.
     *
     * @param commonHeader A vector of strings that contains the header information that is used for sorting.
     */
    virtual void sort(vector<string> commonHeader) = 0;
    virtual TableType getType() = 0;
    virtual ~Table() = default;

    friend class TrueTable;
    friend class FalseTable;
    friend class BooleanTable;
    friend class PQLTable;
};
