#include "Row.h"

Row::Row() = default;

size_t Row::size() const {
    return this->row.size();
}

bool Row::hasColumn(const string &column) const {
    return this->row.find(column) != this->row.end();
}

string Row::getValueAtColumn(const string &column) const {
    if (!hasColumn(column)) {
        throw runtime_error("Column doesnt exist!");
    }

    return this->row.at(column);
}

unordered_map<string, string> Row::getRow() const {
    return this->row;
}

void Row::addEntry(const string &column, const string &value) {
    bool hasColumn = this->hasColumn(column);

    // if this exact entry has been added before, we can return
    if (hasColumn && this->row.at(column) == value) {
        return;
    }

    // Column exist but value doesn't match the entry's value
    if (hasColumn && this->row.at(column) != value) {
        throw runtime_error("This column value already exist!");
    }

    this->row.insert({column, value});
}

bool Row::operator==(const Row &other) const {
    return all_of(row.begin(), row.end(), [&other](const pair<string, string> &columnValuePair) {
        return other.hasColumn(columnValuePair.first) &&
               other.getValueAtColumn(columnValuePair.first) == columnValuePair.second;
    });
}

Row::Row(const string &column, const string &value) {
    this->addEntry(column, value);
}

