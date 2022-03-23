#pragma once

#include "AffectEvaluator.h"

class AffectsTEvaluator : public AffectEvaluator {
private:
    AffectsKBAdapter* affectsKBAdapter;
public:
    explicit AffectsTEvaluator(PKBClient* pkb, AffectsKBAdapter* affectsKBAdapter);

    bool getIntegerIntegerRelation(const string& left, const string& right) override;
    unordered_set<string> getIntegerSynonymRelation(const string& label) override;
    unordered_set<string> getIntegerWildCardRelation(const string& label) override;
    unordered_set<string> getSynonymIntegerRelation(const string& label) override;
    vector<pair<string, string>> getSynonymSynonymRelation() override;
    unordered_set<string> getSynonymWildCardRelation() override;
    unordered_set<string> getWildCardIntegerRelation(const string& label) override;
    unordered_set<string> getWildCardSynonymRelation() override;
    vector<pair<string, string>> getWildCardWildCardRelation() override;
};