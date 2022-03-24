#pragma once

#include "ProcEvaluator.h"

class ProcProcEvaluator : public ProcEvaluator {
public:
    explicit ProcProcEvaluator(PKBClient *pkb);

    Table *evaluateClause(ClauseVariable left, ClauseVariable right) override;

    Table *evaluateWildCardIdentifier(const ClauseVariable &right);

    Table *evaluateWildCardSynonym(ClauseVariable &right);

    Table *evaluateWildCardWildCard();

    virtual unordered_set<string> getWildCardIdentifierRelation(const string &label) = 0;

    virtual unordered_set<string> getWildCardSynonymRelation() = 0;

    virtual vector<pair<string, string>> getWildCardWildCardRelation() = 0;
};