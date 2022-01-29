#pragma once

#include <unordered_map>
#include <string>

using namespace std;

class Row {
private:
    unordered_map<string, string> row;
public:
    Row();

    size_t size() const;
    bool hasColumn(const string& column) const;
    string getValueAtColumn(const string& column) const;
    unordered_map<string, string> getRow() const;

    // adds a column - value to the row
    void addEntry(const string& column, string value);

    // Used for testing purposes to check for strict equality of row
    bool operator==(const Row &other) const;
};

