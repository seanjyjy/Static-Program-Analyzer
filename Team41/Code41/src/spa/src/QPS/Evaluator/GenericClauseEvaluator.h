#pragma once

#include "Evaluator.h"

class GenericClauseEvaluator : public Evaluator {
public:
    Table* evaluate(QueryClause clause) override;
    virtual Table* evaluateClause(ClauseVariable left, ClauseVariable right) = 0;

    friend class ProcEvaluator;
    friend class StmtStmtEvaluator;
    friend class ModifiesUsesSEvaluator;
};