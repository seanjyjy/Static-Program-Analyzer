#pragma once
#include "QPS/Table/Table.h"
#include "QPS/Table/FalseTable.h"
#include "QPS/Table/TrueTable.h"
#include "QPS/Table/PQLTable.h"
#include "PKB/PKB.h"
#include "QPS/ClauseVariable.h"
#include "QPS/QueryClause.h"
#include "QPS/Evaluator/EvaluatorUtils.h"
#include "Exception/SemanticException.h"

using namespace std;

class Evaluator {
public:
    /**
     * Based on the clause, it will determine which evaluator to use to retrieve information from the pkb
     * to build the resultant table.
     *
     * @param clause A QueryClause that contains the clause type, and two clause variable.
     * @param pkb A knowledge base based on the Source Program.
     * @return A table that contains information based on the query that was executed.
     */
    virtual Table* evaluate(QueryClause clause, PKB *pkb) = 0;
};