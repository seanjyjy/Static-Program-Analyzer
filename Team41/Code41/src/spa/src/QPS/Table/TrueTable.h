#pragma once

#include "Table.h"

class TrueTable : public Table {
public:
    TrueTable();
    ~TrueTable();

    bool isEmpty() override;
    Table* mergeJoin(Table* intermediatePQLTable) override;
    vector<const Row *> getRows() override;
    void addRow(const Row* row) override;
    Header getHeader() const override;
    bool hasRow(const Row* row) override;
    size_t size() override;
    virtual unordered_set<string> getColumn(string columnName) override;
    void sort(const Header& commonHeader) override;
    TableType getType() override;
};
