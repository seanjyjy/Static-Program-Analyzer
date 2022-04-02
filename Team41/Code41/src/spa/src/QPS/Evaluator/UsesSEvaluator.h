#pragma once

#include "ModifiesUsesSEvaluator.h"

class UsesSEvaluator : public ModifiesUsesSEvaluator {
public:
    explicit UsesSEvaluator(PKBClient *pkb);

    /**
     * Construct a table that captures the UsesS relation.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @param canSimplify A boolean representing if this clause can be converted into a boolean result.
     * @return A table that contains information based on the UsesS relation.
     */
    Table *evaluateClauseFurther(ClauseVariable &left, ClauseVariable &right, bool canSimplify) override;

    /**
     * @param leftLabel A statement number.
     * @param rightLabel A variable.
     * @return True if this statement number uses this variable else false
     */
    bool getIntegerIdentifierRelation(const string &leftLabel, const string &rightLabel) override;

    /**
     * @param label A statement number.
     * @return A set of variables that is being used in this statement number.
     */
    unordered_set<string> getIntegerSynonymRelation(const string &label) override;

    /**
     * @param label A statement number.
     * @return True if this statement uses any variable else false.
     */
    bool getIntegerWildCardRelation(const string &label) override;

    /**
     * @param label A variable.
     * @return A set of stmts that uses this variable
     */
    unordered_set<string> getSynonymIdentifierRelation(const string &label) override;

    /**
     * @return A vector of stmt pairs that fulfils the synonym synonym relation of UsesS relation
     */
    vector<pair<string, string>> getSynonymSynonymRelation() override;

    /**
     * @return A set of statements that fulfils the synonym wildcard relation of UsesS relation
     */
    unordered_set<string> getSynonymWildCardRelation() override;

    /**
     * @param label A variable.
     * @return True if a statement uses this variable else return false.
     */
    bool getWildCardIdentifierRelation(const string &label) override;

    /**
     * @return True if the wildcard wildcard relation is not empty for UsesS relation.
     */
    bool getWildCardWildCardRelation() override;
};