#pragma once
#include "Table/Table.h"
#include "QueryObject.h"
#include "PKB/PKBClient.h"

#include <unordered_set>

class QueryProjector {
private:
    Table* table;
    SelectTarget selectTarget;
    PKBClient* pkb;
    bool isValid;
    unordered_set<string> getBooleanResult();
    unordered_set<string> getTupleResult();
    string getProjectionFromRow(const Row* row, Selectable* target);
public:
    QueryProjector(SelectTarget select, Table* table, PKBClient* pkb, bool isValid);
    ~QueryProjector();

    unordered_set<string> getResult();
};