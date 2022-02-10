#pragma once

#include "Evaluator.h"

class FollowsTEvaluator {
private:
    static Table* evaluateIntegerInteger(PKB* pkb, ClauseVariable left, ClauseVariable right);
    static Table* evaluateIntegerSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right);
    static Table* evaluateIntegerWildCard(PKB* pkb, ClauseVariable left);
    static Table* evaluateSynonymInteger(PKB* pkb, ClauseVariable left, ClauseVariable right);
    static Table* evaluateSynonymSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right);
    static Table* evaluateSynonymWildCard(PKB* pkb, ClauseVariable left);
    static Table* evaluateWildCardInteger(PKB* pkb, ClauseVariable right);
    static Table* evaluateWildCardSynonym(PKB* pkb, ClauseVariable right);
    static Table* evaluateWildCardWildCard(PKB* pkb);
public:
    static Table* evaluate(QueryClause clause, PKB *pkb);
};