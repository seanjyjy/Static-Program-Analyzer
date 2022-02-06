#pragma once
#include "QPS/Table/Table.h"
#include "QPS/Table/FalseTable.h"
#include "QPS/Table/TrueTable.h"
#include "QPS/Table/PQLTable.h"
#include "PKB/PKB.h"
#include "QPS/ClauseVariable.h"
#include "QPS/QueryClause.h"

using namespace std;

class Evaluator {
public:
    // string to be changed once a clause side is created
    virtual Table* evaluate(QueryClause clause, PKB *pkb) = 0;
};