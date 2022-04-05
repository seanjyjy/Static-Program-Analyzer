#pragma once

#include "AffectEvaluator.h"

class AffectsEvaluator : public AffectEvaluator {
public:
    explicit AffectsEvaluator(PKBClient *pkb, AffectsKBProxy *affectsKBProxy);

    /**
     * @param left A statement number.
     * @param right A statement number.
     * @return True if left statement directly affects right statement else false.
     */
    bool getIntegerIntegerRelation(const string &left, const string &right) override;

    /**
     * @param label A statement number.
     * @return A set of statements that is directly affected by this statement.
     */
    unordered_set<string> getIntegerSynonymRelation(const string &label) override;

    /**
     * @param label A statement number.
     * @return A set of statements that is directly affecting this statement.
     */
    unordered_set<string> getSynonymIntegerRelation(const string &label) override;

    /**
     * @return A vector of stmts pairs where a stmt directly affects another stmt.
     */
    vector<pair<string, string>> getSynonymSynonymRelation() override;

    /**
     * @return A set of statements that are directly affecting some stmt.
     */
    unordered_set<string> getSynonymWildCardRelation() override;

    /**
     * @return A set of statements that are directly affected by some stmt.
     */
    unordered_set<string> getWildCardSynonymRelation() override;
};