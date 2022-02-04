#pragma once

#include "Table.h"

class FalseTable : public Table {
public:
    FalseTable();
    bool isEmpty() override;
    Table* mergeJoin(Table* intermediatePQLTable) override;
    vector<const Row *> getRows() override;
    void addRow(const Row* row) override;
    Header getHeader() const override;
    bool hasRow(const Row* row) override;
    size_t size() override;
};