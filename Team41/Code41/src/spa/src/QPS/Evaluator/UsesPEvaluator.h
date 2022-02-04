#pragma once

#include "Evaluator.h"

// TODO: to change when i know what is the clause type
class UsesPEvaluator {
private:
    Table* evaluateUsesIdentifierIdentifier(PKB* pkb);
    Table* evaluateUsesIdentifierSynonym(PKB* pkb);
    Table* evaluateUsesIdentifierWildCard(PKB* pkb);
    Table* evaluateUsesSynonymIdentifier(PKB* pkb);
    Table* evaluateUsesSynonymSynonym(PKB* pkb);
    Table* evaluateUsesSynonymWildCard(PKB* pkb);
public:
    static Table* evaluate(string clause, PKB *pkb);
};