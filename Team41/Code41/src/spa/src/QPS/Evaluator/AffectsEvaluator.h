#pragma once

#include "AffectEvaluator.h"

class AffectsEvaluator : public AffectEvaluator {
private:
    AffectsKBAdapter* affectsKBAdapter;
public:
    explicit AffectsEvaluator(PKBClient* pkb, AffectsKBAdapter* affectsKBAdapter);

    bool getIntegerIntegerRelation(const string& left, const string& right);
    unordered_set<string> getIntegerSynonymRelation(const string& label);
    unordered_set<string> getIntegerWildCardRelation(const string& label);
    unordered_set<string> getSynonymIntegerRelation(const string& label);
    vector<pair<string, string>> getSynonymSynonymRelation();
    unordered_set<string> getSynonymWildCardRelation();
    unordered_set<string> getWildCardIntegerRelation(const string& label);
    unordered_set<string> getWildCardSynonymRelation();
    vector<pair<string, string>> getWildCardWildCardRelation();
};