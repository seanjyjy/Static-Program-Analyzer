#pragma once

#include "Table/Table.h"
#include "QueryObject.h"
#include "PKB/PKBClient.h"

#include <unordered_set>

class QueryResult {
private:
    SelectTarget selectTarget;
    Table* table;
    bool isValidQuery;
public:
    QueryResult(SelectTarget select, Table* table, bool isValidQuery = true);

    bool isBoolean();
    bool isTuple();
    bool isEmpty();
    bool isValid() const;
    Table* getTable();
    vector<Selectable> getSelectables();
};