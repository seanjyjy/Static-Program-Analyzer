#pragma once

#include "Evaluator.h"

class GenericClauseEvaluator : public Evaluator {
public:
    GenericClauseEvaluator(PKBClient* pkb);
    GenericClauseEvaluator(PKBClient* pkb, NextKBAdapter* nextKbAdapter);
    Table* evaluate(QueryClause clause) override;
    // TODO REMOVE THIS IN THE FUTURE
    Table* evaluate(PatternClause clause) override;
    virtual Table* evaluateClause(ClauseVariable left, ClauseVariable right) = 0;

    friend class ProcEvaluator;
    friend class StmtStmtEvaluator;
    friend class ModifiesUsesSEvaluator;
};