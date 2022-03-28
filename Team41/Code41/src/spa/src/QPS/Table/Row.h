#pragma once

#include <unordered_map>
#include <string>
#include <stdexcept>
#include <utility>
#include <algorithm>

using namespace std;

class Row {
private:
    unordered_map<string, string> row;
public:
    Row();

    Row(const string &column, const string &value);

    /**
     * Retrieves the number of entries in the current row.
     *
     * @return the number of entries in current row.
     */
    size_t size() const;

    /**
     * Checks if the current row contains a certain column.
     *
     * @param column The name of the column.
     * @return True if current row contains the name of a column else return false.
     */
    bool hasColumn(const string &column) const;

    /**
     * Retrieves a values from a certain column.
     *
     * @param column The name of the column.
     * @return The string value at that particular column if exist, else throw an error.
     */
    string getValueAtColumn(const string &column) const;

    /**
     * Retrieves the current row.
     *
     * @return The current row.
     */
    unordered_map<string, string> getRow() const;

    /**
     * Adds the value to this row on a particular column.
     *
     * @param column The name of the column.
     * @param value The value to be added to a certain column
     */
    void addEntry(const string &column, const string &value);

    /**
     * Compares if two rows are equal.
     *
     * @note Used for testing purposes to check for strict equality of row
     * @param other The other row to compare to.
     * @return True if both rows are equal else false.
     */
    bool operator==(const Row &other) const;
};

