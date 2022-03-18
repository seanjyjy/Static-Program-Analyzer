#pragma once
#include "Table/Table.h"
#include "QueryResult.h"
#include "PKB/PKBClient.h"

#include <unordered_set>

class QueryProjector {
private:
    QueryResult queryResult;

    unordered_set<string> getBooleanResult();
    unordered_set<string> getTupleResult();
    string getProjectionFromRow(const Row* row, Selectable* target);
public:
    explicit QueryProjector(const QueryResult& queryResult);
    ~QueryProjector();

    unordered_set<string> getResult();
};