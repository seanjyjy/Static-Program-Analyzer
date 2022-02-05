#pragma once

#include "Evaluator.h"

// TODO: to change when i know what is the clause type
class UsesPEvaluator {
private:
    static Table* evaluateIdentifierIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right);
    static Table* evaluateIdentifierSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right);
    static Table* evaluateIdentifierWildCard(PKB* pkb, ClauseVariable left);
    static Table* evaluateSynonymIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right);
    static Table* evaluateSynonymSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right);
    static Table* evaluateSynonymWildCard(PKB* pkb, ClauseVariable left);
public:
    static Table* evaluate(QueryClause clause, PKB *pkb);
};