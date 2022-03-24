#include "Row.h"
#include <stdexcept>
#include <utility>

Row::Row() = default;

size_t Row::size() const {
    return this->row.size();
}

bool Row::hasColumn(const std::string &column) const {
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

void Row::addEntry(const string &column, string value) {
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
    for (auto &columnValuePair: this->row) {
        if (!other.hasColumn(columnValuePair.first)) {
            return false;
        }

        if (other.getValueAtColumn(columnValuePair.first) != columnValuePair.second) {
            return false;
        }
    }
    return true;
}

Row::Row(const string &column, string value) {
    this->addEntry(column, std::move(value));
}

