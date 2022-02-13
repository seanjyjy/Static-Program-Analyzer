#pragma once
#include "QPS/Table/Table.h"
#include "QPS/Table/FalseTable.h"
#include "QPS/Table/TrueTable.h"
#include "QPS/Table/PQLTable.h"
#include "PKB/PKB.h"
#include "QPS/ClauseVariable.h"
#include "QPS/QueryClause.h"
#include "QPS/Evaluator/EvaluatorUtils.h"

using namespace std;

class Evaluator {
public:
    virtual Table* evaluate(QueryClause clause, PKB *pkb) = 0;
};