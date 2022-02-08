#pragma once

#include "Evaluator.h"

class ModifiesSEvaluator {
private:
    static Table* evaluateIntegerIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right);
    static Table* evaluateIntegerSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right);
    static Table* evaluateIntegerWildCard(PKB* pkb, ClauseVariable left);
    static Table* evaluateSynonymIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right);
    static Table* evaluateSynonymSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right);
    static Table* evaluateSynonymWildCard(PKB* pkb, ClauseVariable left);
public:
    static Table* evaluate(QueryClause clause, PKB *pkb);
};