#pragma once

#include "ProcEvaluator.h"

class ModifiesUsesPEvaluator : public ProcEvaluator {
public:
    explicit ModifiesUsesPEvaluator(PKBClient *pkb);

    Table *evaluateClause(ClauseVariable &left, ClauseVariable &right) override;

    friend class ModifiesPEvaluator;

    friend class UsesPEvaluator;
};
