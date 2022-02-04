#pragma once

#include "Evaluator.h"

// TODO: to change when i know what is the clause type
class UsesSEvaluator {
private:
    static Table* evaluateUsesIntegerIdentifier(PKB* pkb);
    static Table* evaluateUsesIntegerSynonym(PKB* pkb);
    static Table* evaluateUsesIntegerWildCard(PKB* pkb);
    static Table* evaluateUsesSynonymIdentifier(PKB* pkb);
    static Table* evaluateUsesSynonymSynonym(PKB* pkb);
    static Table* evaluateUsesSynonymWildCard(PKB* pkb);
public:
    static Table* evaluate(string clause, PKB *pkb);
};