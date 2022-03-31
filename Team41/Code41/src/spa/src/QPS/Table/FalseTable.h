#pragma once

#include "BooleanTable.h"

class FalseTable : public BooleanTable {
public:
    FalseTable();

    ~FalseTable();

    // This is a stub method
    bool isEmpty() override;

    // This is a stub method
    Table *mergeJoin(Table *intermediatePQLTable) override;

    bool isFalseTable() override;
};