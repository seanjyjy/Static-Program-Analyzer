#pragma once
#include "QPS/Table/Table.h"
#include "QPS/Table/FalseTable.h"
#include "QPS/Table/TrueTable.h"
#include "QPS/Table/PQLTable.h"
#include "PKB/PKB.h"
#include "QPS/QueryDeclaration.h"

class SelectSynonymEvaluator {
private:
    static unordered_set<string> getResultViaType(QueryDeclaration::design_entity_type type, PKB *pkb);
public:
    static Table* evaluate(const QueryDeclaration selectSynonym, PKB *pkb);
};
