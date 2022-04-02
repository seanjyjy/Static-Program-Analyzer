#pragma once

#include "QPS/Table/Table.h"
#include "QPS/Table/FalseTable.h"
#include "QPS/Table/TrueTable.h"
#include "QPS/Table/PQLTable.h"
#include "PKB/PKBClient.h"
#include "QPS/QueryDeclaration.h"
#include "Exception/SemanticException.h"

class SelectSynonymEvaluator {
private:
    PKBClient *pkb;
public:
    explicit SelectSynonymEvaluator(PKBClient *pkb);

    /**
     * Depending on the select synonym in query declaration , get all values of that synonym from the pkb.
     *
     * @param selectSynonym A QueryDeclaration that contains the synonym to select from.
     * @return A table depending on the select synonym.
     */
    Table *evaluate(const QueryDeclaration &selectSynonym);
};
