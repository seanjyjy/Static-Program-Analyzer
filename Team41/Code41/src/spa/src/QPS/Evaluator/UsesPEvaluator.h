#pragma once

#include "ModifiesUsesPEvaluator.h"

class UsesPEvaluator : public ModifiesUsesPEvaluator {
public:
    explicit UsesPEvaluator(PKBClient *pkb);

    bool getIdentifierIdentifierRelation(const string &leftLabel, const string &rightLabel) override;

    unordered_set<string> getIdentifierSynonymRelation(const string &label) override;

    bool getIdentifierWildCardRelation(const string &label) override;

    unordered_set<string> getSynonymIdentifierRelation(const string &label) override;

    vector<pair<string, string>> getSynonymSynonymRelation() override;

    unordered_set<string> getSynonymWildCardRelation() override;

    bool getWildCardIdentifierRelation(const string &label) override;

    bool getWildCardWildCardRelation() override;
};