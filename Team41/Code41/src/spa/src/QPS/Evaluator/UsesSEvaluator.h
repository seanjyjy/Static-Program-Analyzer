#pragma once

#include "Evaluator.h"

class UsesSEvaluator {
    Table* evaluateUsesIntegerIdentifier(PKB* pkb);
    Table* evaluateUsesIntegerSynonym(PKB* pkb);
    Table* evaluateUsesIntegerWildCard(PKB* pkb);
    Table* evaluateUsesSynonymIdentifier(PKB* pkb);
    Table* evaluateUsesSynonymSynonym(PKB* pkb);
    Table* evaluateUsesSynonymWildCard(PKB* pkb);
};