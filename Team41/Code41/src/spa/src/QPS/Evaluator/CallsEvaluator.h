#pragma once

#include "ProcProcEvaluator.h"

class CallsEvaluator : ProcProcEvaluator {
private:
    PKBClient* pkb;

    bool getIdentifierIdentifierRelation(const string& leftLabel, const string& rightLabel) override;
    unordered_set<string> getIdentifierSynonymRelation(const string& label) override;
    unordered_set<string> getIdentifierWildCardRelation(const string& label) override;
    unordered_set<string> getSynonymIdentifierRelation(const string& label) override;
    vector<pair<string, string>> getSynonymSynonymRelation() override;
    unordered_set<string> getSynonymWildCardRelation() override;
    unordered_set<string> getWildCardIdentifierRelation(const string& label) override;
    unordered_set<string> getWildCardSynonymRelation() override;
    vector<pair<string, string>> getWildCardWildCardRelation() override;
public:
    explicit CallsEvaluator(PKBClient* pkb);
};
