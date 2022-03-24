#pragma once

#include "IfWhilePatternEvaluator.h"
#include "Common/TNode.h"
#include "../PatternClause.h"

class WhilePatternEvaluator : public IfWhilePatternEvaluator {
public:
    explicit WhilePatternEvaluator(PKBClient *pkb);

    unordered_set<string> getWildCardWildCardRelation() override;

    unordered_set<string> getIdentifierWildCardRelation(const string &label) override;

    vector<pair<string, string>> getSynonymWildCardRelation() override;
};