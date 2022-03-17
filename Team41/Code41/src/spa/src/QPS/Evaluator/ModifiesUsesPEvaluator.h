#pragma once

#include "ProcEvaluator.h"

class ModifiesUsesPEvaluator : public ProcEvaluator  {
public:
    Table* evaluateClause(ClauseVariable left, ClauseVariable right) override;
};
