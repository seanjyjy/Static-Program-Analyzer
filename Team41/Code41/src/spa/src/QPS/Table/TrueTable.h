#pragma once

#include "Table.h"

class TrueTable : public Table {
public:
    TrueTable();
    ~TrueTable();

    // This is a stub method
    bool isEmpty() override;

    // This is a stub method
    Table* mergeJoin(Table* intermediatePQLTable) override;

    // This is a stub method
    vector<const Row *> getRows() override;

    // This is a stub method
    void addRow(const Row* row) override;

    // This is a stub method
    Header getHeader() const override;

    // This is a stub method
    bool hasRow(const Row* row) override;

    // This is a stub method
    size_t size() override;

    // This is a stub method
    virtual unordered_set<string> getColumn(string columnName) override;

    // This is a stub method
    void sort(vector<string> commonHeader) override;

    // This is a stub method
    TableType getType() override;
};
