#pragma once

#include "ModifiesUsesPEvaluator.h"

class ModifiesPEvaluator : public ModifiesUsesPEvaluator {
public:
    explicit ModifiesPEvaluator(PKBClient *pkb);

    bool getIdentifierIdentifierRelation(const string &leftLabel, const string &rightLabel) override;

    unordered_set<string> getIdentifierSynonymRelation(const string &label) override;

    unordered_set<string> getIdentifierWildCardRelation(const string &label) override;

    unordered_set<string> getSynonymIdentifierRelation(const string &label) override;

    vector<pair<string, string>> getSynonymSynonymRelation() override;

    unordered_set<string> getSynonymWildCardRelation() override;

    /**
     * This is a hack to optimize unused clause in the PQL. This hack is used in @ModifiesS @ModifiesP @UsesS @UseP
     * (syn, ident) -> (wildcard, ident)
     * (syn, syn) -> (wildcard, wildcard)
     * (syn, wildcard) -> (wildcard, wildcard)
     */
    unordered_set<string> getWildCardIdentifierRelation(const string &label) override;

    vector<pair<string, string>> getWildCardWildCardRelation() override;
};
