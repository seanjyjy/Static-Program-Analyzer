#pragma once

#include "Evaluator.h"

class UsesPEvaluator {
public:
    // TODO: to change when i know what is the clause type
    Table* evaluateUsesIdentifierIdentifier(PKB* pkb);
    Table* evaluateUsesIdentifierSynonym(PKB* pkb);
    Table* evaluateUsesIdentifierWildCard(PKB* pkb);
    Table* evaluateUsesSynonymIdentifier(PKB* pkb);
    Table* evaluateUsesSynonymSynonym(PKB* pkb);
    Table* evaluateUsesSynonymWildCard(PKB* pkb);
};