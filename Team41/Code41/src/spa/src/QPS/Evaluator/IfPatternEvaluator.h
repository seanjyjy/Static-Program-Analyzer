#pragma once

#include "IfWhilePatternEvaluator.h"
#include "Common/TNode.h"
#include "../PatternClause.h"

class IfPatternEvaluator : IfWhilePatternEvaluator {
private:
    PKBClient* pkb;
public:
    explicit IfPatternEvaluator(PKBClient* pkb);

    unordered_set<string> getWildCardWildCardRelation() override;
    unordered_set<string> getIdentifierWildCardRelation(const string& label) override;
    vector<pair<string, string>> getSynonymWildCardRelation() override;
};