#pragma once

#include "ModifiesUsesSEvaluator.h"

class ModifiesSEvaluator : public ModifiesUsesSEvaluator {
public:
    explicit ModifiesSEvaluator(PKBClient *pkb);

    /**
     * Construct a table that captures the ModifiesS relation.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @param canSimplify A boolean representing if this clause can be converted into a boolean result.
     * @return A table that contains information based on the ModifiesS relation.
     */
    Table *evaluateClauseFurther(ClauseVariable &left, ClauseVariable &right, bool canSimplify) override;

    /**
     * @param leftLabel A statement number.
     * @param rightLabel A variable.
     * @return True if this statement number modifies this variable else false
     */
    bool getIntegerIdentifierRelation(const string &leftLabel, const string &rightLabel) override;

    /**
      * @param label A statement number.
      * @return A set of variables that is being modified in this statement number.
      */
    unordered_set<string> getIntegerSynonymRelation(const string &label) override;

    /**
     * @param label A statement number.
     * @return True if this statement modifies any variable else false.
     */
    bool getIntegerWildCardRelation(const string &label) override;

    /**
     * @param label A variable.
     * @return A set of stmts that modifies this variable
     */
    unordered_set<string> getSynonymIdentifierRelation(const string &label) override;

    /**
     * @return A vector of stmt pairs that fulfils the synonym synonym relation of ModifiesS relation
     */
    vector<pair<string, string>> getSynonymSynonymRelation() override;

    /**
     * @return A set of statements that fulfils the synonym wildcard relation of ModifiesS relation
     */
    unordered_set<string> getSynonymWildCardRelation() override;

    /**
     * @param label A variable.
     * @return True if a statement modifies this variable else return false.
     */
    bool getWildCardIdentifierRelation(const string &label) override;

    /**
     * @return True if the wildcard wildcard relation is not empty for ModifiesS relation.
     */
    bool getWildCardWildCardRelation() override;
};