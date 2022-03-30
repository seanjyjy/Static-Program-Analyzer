#pragma once

#include "ProcEvaluator.h"

class ProcProcEvaluator : public ProcEvaluator {
public:
    explicit ProcProcEvaluator(PKBClient *pkb);

    Table *evaluateClause(ClauseVariable &left, ClauseVariable &right, bool canSimplify) override;

    Table *evaluateWildCardSynonym(ClauseVariable &right, bool canSimplify);

    virtual unordered_set<string> getWildCardSynonymRelation() = 0;

    friend class CallsEvaluator;

    friend class CallsTEvaluator;
};