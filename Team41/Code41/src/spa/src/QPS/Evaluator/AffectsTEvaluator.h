#pragma once

#include "AffectEvaluator.h"

class AffectsTEvaluator : public AffectEvaluator {
public:
    explicit AffectsTEvaluator(PKBClient *pkb, AffectsKBAdapter *affectsKBAdapter);

    /**
     * @param left A statement number.
     * @param right A statement number.
     * @return True if left statement directly or indirectly affects right statement else false.
     */
    bool getIntegerIntegerRelation(const string &left, const string &right) override;

    /**
     * @param label A statement number.
     * @return A set of statements that is directly or indirectly affected by this statement.
     */
    unordered_set<string> getIntegerSynonymRelation(const string &label) override;

    /**
     * @param label A statement number.
     * @return A set of statements that is directly or indirectly affecting this statement.
     */
    unordered_set<string> getSynonymIntegerRelation(const string &label) override;

    /**
     * @return A vector of stmts pairs where a stmt directly or indirectly affects another stmt.
     */
    vector<pair<string, string>> getSynonymSynonymRelation() override;

    /**
     * @return A set of statements that are directly or indirectly affecting some stmt.
     */
    unordered_set<string> getSynonymWildCardRelation() override;

    /**
     * @return A set of statements that are directly or indirectly affected by some stmt.
     */
    unordered_set<string> getWildCardSynonymRelation() override;
};