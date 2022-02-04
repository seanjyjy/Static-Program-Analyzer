#pragma once

#include "Evaluator.h"

// TODO: to change when i know what is the clause type
class ModifiesSEvaluator {
private:
    static Table* evaluateModifiesIntegerIdentifier(PKB* pkb);
    static Table* evaluateModifiesIntegerSynonym(PKB* pkb);
    static Table* evaluateModifiesIntegerWildCard(PKB* pkb);
    static Table* evaluateModifiesSynonymIdentifier(PKB* pkb);
    static Table* evaluateModifiesSynonymSynonym(PKB* pkb);
    static Table* evaluateModifiesSynonymWildCard(PKB* pkb);
public:
    static Table* evaluate(string clause, PKB *pkb);
};