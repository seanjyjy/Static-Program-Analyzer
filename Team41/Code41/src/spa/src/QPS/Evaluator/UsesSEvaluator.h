#pragma once

#include "Evaluator.h"

// TODO: to change when i know what is the clause type
class UsesSEvaluator {
private:
    Table* evaluateUsesIntegerIdentifier(PKB* pkb);
    Table* evaluateUsesIntegerSynonym(PKB* pkb);
    Table* evaluateUsesIntegerWildCard(PKB* pkb);
    Table* evaluateUsesSynonymIdentifier(PKB* pkb);
    Table* evaluateUsesSynonymSynonym(PKB* pkb);
    Table* evaluateUsesSynonymWildCard(PKB* pkb);
public:
    static Table* evaluate(string clause, PKB *pkb);
};