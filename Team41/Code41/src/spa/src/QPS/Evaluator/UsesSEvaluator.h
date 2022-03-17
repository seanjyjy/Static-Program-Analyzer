#pragma once

#include "ModifiesUsesSEvaluator.h"

class UsesSEvaluator : ModifiesUsesSEvaluator {
public:
    explicit UsesSEvaluator(PKBClient* pkb);

    Table* evaluateClauseFurther(ClauseVariable left, ClauseVariable right) override;

    bool getIntegerIdentifierRelation(const string& leftLabel, const string& rightLabel) override;
    unordered_set<string> getIntegerSynonymRelation(const string& label) override;
    unordered_set<string> getIntegerWildCardRelation(const string& label) override;
    unordered_set<string> getSynonymIdentifierRelation(const string& label) override;
    vector<pair<string, string>> getSynonymSynonymRelation() override;
    unordered_set<string> getSynonymWildCardRelation() override;
};