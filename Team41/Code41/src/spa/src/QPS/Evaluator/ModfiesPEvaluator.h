#pragma once

#include "Evaluator.h"

// TODO: to change when i know what is the clause type
class ModifiesPEvaluator {
private:
    static Table* evaluateModifiesIdentifierIdentifier(PKB* pkb);
    static Table* evaluateModifiesIdentifierSynonym(PKB* pkb);
    static Table* evaluateModifiesIdentifierWildCard(PKB* pkb);
    static Table* evaluateModifiesSynonymIdentifier(PKB* pkb);
    static Table* evaluateModifiesSynonymSynonym(PKB* pkb);
    static Table* evaluateModifiesSynonymWildCard(PKB* pkb);
public:
    static Table* evaluate(string clause, PKB *pkb);
};
