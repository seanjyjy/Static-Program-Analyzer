#pragma once

#include "Table/Table.h"
#include "QueryObject.h"
#include "PKB/PKBClient.h"

#include <unordered_set>

class QueryResult {
private:
    SelectTarget selectTarget;
    Table* table;
    PKBClient* pkb;
    bool isValidQuery;
public:
    QueryResult(SelectTarget select, Table* table, PKBClient* pkb, bool isValidQuery);

    bool isBoolean();
    bool isTuple();
    bool isEmpty();
    bool isValid() const;
    PKBClient* getPKB();
    Table* getTable();
    vector<Selectable> getSelectables();
};