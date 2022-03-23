#pragma once

#include <stdexcept>

#include "Table.h"

class BooleanTable : public Table {
    // This is a stub method
    vector<const Row *> getRows() override;

    // This is a stub method
    void addRow(const Row *row) override;

    // This is a stub method
    Header getHeader() const override;

    // This is a stub method
    bool hasRow(const Row *row) override;

    // This is a stub method
    size_t size() override;

    // This is a stub method
    unordered_set<string> getColumn(string columnName) override;

    // This is a stub method
    void sort(vector<string> commonHeader) override;

    // This is a stub method
    unordered_set<string> getColumns(vector<string> columnNames) override;

    friend class Truetable;

    friend class FalseTable;
};