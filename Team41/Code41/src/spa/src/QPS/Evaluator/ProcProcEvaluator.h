#pragma once

#include "ProcEvaluator.h"

class ProcProcEvaluator : public ProcEvaluator {
public:
    explicit ProcProcEvaluator(PKBClient *pkb);

    Table *evaluateClause(ClauseVariable &left, ClauseVariable &right) override;

    Table *evaluateWildCardSynonym(ClauseVariable &right);

    virtual unordered_set<string> getWildCardSynonymRelation() = 0;

    friend class CallsEvaluator;

    friend class CallsTEvaluator;
};