#pragma once
#include "QPS/Table/Table.h"
#include "QPS/Table/FalseTable.h"
#include "QPS/Table/TrueTable.h"
#include "QPS/Table/PQLTable.h"
#include "PKB/PKB.h"
#include "QPS/QueryDeclaration.h"

class SelectSynonymEvaluator {
private:
    /**
     * Depending on the query declaration select type, select all selected type from the pkb.
     *
     * @param type QueryDeclaration type.
     * @param pkb A knowledge base based on the Source Program.
     * @return A table depending on the select type.
     */
    static unordered_set<string> getResultViaType(QueryDeclaration::design_entity_type type, PKB *pkb);
public:
    /**
     * Depending on the select synonym in query declaration , get all values of that synonym from the pkb.
     *
     * @param selectSynonym A QueryDeclaration that contains the synonym to select from.
     * @param pkb A knowledge base based on the Source Program.
     * @return A table depending on the select synonym.
     */
    static Table* evaluate(const QueryDeclaration selectSynonym, PKB *pkb);
};
