#pragma once

#include <vector>

using namespace std;

#include "TempClause.h"
#include "QPS/Optimizer/PKBAdapter.h"
#include "QPS/Optimizer/TableEstimate.h"

// TODO maybe subclass clause groups into static/dynamic?
class ClauseGroup {
private:
    vector<TempClause> clauses;
    vector<bool> isUsed;
    TableEstimate table;
public:
    ClauseGroup(PKBAdapter pkbAdapter);
    void addClause(const TempClause &c);
    void sortClauses();
    bool hasNextClause();
    TempClause getNextClauseStatic(); // reads clauses in the same order they were added
    TempClause getNextClauseDynamic(); // gets the clause resulting in the smallest intermediate table
    long getScore();
};
