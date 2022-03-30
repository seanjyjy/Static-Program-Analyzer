#pragma once

#include "Evaluator.h"

class GenericClauseEvaluator : public Evaluator {
public:
    explicit GenericClauseEvaluator(PKBClient *pkb);

    Table *evaluate(const QueryClause &clause);

    virtual Table *evaluateClause(ClauseVariable &left, ClauseVariable &right, bool canSimplify) = 0;

    friend class ProcEvaluator;

    friend class StmtStmtEvaluator;

    friend class ModifiesUsesSEvaluator;
};