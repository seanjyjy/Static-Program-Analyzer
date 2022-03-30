#pragma once

#include "ModifiesUsesSEvaluator.h"

class UsesSEvaluator : public ModifiesUsesSEvaluator {
public:
    explicit UsesSEvaluator(PKBClient *pkb);

    Table *evaluateClauseFurther(ClauseVariable &left, ClauseVariable &right, bool canSimplify) override;

    bool getIntegerIdentifierRelation(const string &leftLabel, const string &rightLabel) override;

    unordered_set<string> getIntegerSynonymRelation(const string &label) override;

    bool getIntegerWildCardRelation(const string &label) override;

    unordered_set<string> getSynonymIdentifierRelation(const string &label) override;

    vector<pair<string, string>> getSynonymSynonymRelation() override;

    unordered_set<string> getSynonymWildCardRelation() override;

    bool getWildCardIdentifierRelation(const string &label) override;

    bool getWildCardWildCardRelation() override;
};