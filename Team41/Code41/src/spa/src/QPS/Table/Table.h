#pragma once

#include <vector>
#include <string>
#include <unordered_set>

#include "Row.h"

using namespace std;

using Header = unordered_set<string>;
using Iterator = std::vector<const Row *>::iterator;

class Table {
public:
    enum TableType {
        TrueTable, FalseTable, PQLTable
    };
    virtual bool isEmpty() = 0;
    virtual Table* mergeJoin(Table* intermediatePQLTable) = 0;
    virtual vector<const Row *> getRows() = 0;
    virtual void addRow(const Row* row) = 0;
    virtual Header getHeader() const = 0;
    virtual bool hasRow(const Row* row) = 0;
    virtual size_t size() = 0;
    virtual unordered_set<string> getColumn(string columnName) = 0;
    virtual void sort(const Header& commonHeader) = 0;
    virtual TableType getType() = 0;
    virtual ~Table() = default;

    friend class TrueTable;
    friend class FalseTable;
    friend class PQLTable;
};
