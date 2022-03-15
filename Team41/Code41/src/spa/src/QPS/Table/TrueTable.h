#pragma once

#include "BooleanTable.h"

class TrueTable : public BooleanTable {
public:
    TrueTable();
    ~TrueTable();

    // This is a stub method
    bool isEmpty() override;

    // This is a stub method
    Table* mergeJoin(Table* intermediatePQLTable) override;

    // This is a stub method
    TableType getType() override;
};
