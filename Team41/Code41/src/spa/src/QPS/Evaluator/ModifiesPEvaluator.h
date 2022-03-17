#pragma once

#include "ModifiesUsesPEvaluator.h"

class ModifiesPEvaluator : ModifiesUsesPEvaluator {
public:
    explicit ModifiesPEvaluator(PKBClient* pkb);

    bool getIdentifierIdentifierRelation(const string& leftLabel, const string& rightLabel) override;
    unordered_set<string> getIdentifierSynonymRelation(const string& label) override;
    unordered_set<string> getIdentifierWildCardRelation(const string& label) override;
    unordered_set<string> getSynonymIdentifierRelation(const string& label) override;
    vector<pair<string, string>> getSynonymSynonymRelation() override;
    unordered_set<string> getSynonymWildCardRelation() override;
};
