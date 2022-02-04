#pragma once

#include "Evaluator.h"

// TODO: to change when i know what is the clause type
class UsesPEvaluator {
private:
    static Table* evaluateUsesIdentifierIdentifier(PKB* pkb);
    static Table* evaluateUsesIdentifierSynonym(PKB* pkb);
    static Table* evaluateUsesIdentifierWildCard(PKB* pkb);
    static Table* evaluateUsesSynonymIdentifier(PKB* pkb);
    static Table* evaluateUsesSynonymSynonym(PKB* pkb);
    static Table* evaluateUsesSynonymWildCard(PKB* pkb);
public:
    static Table* evaluate(string clause, PKB *pkb);
};