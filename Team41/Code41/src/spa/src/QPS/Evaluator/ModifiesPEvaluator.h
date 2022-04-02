#pragma once

#include "ModifiesUsesPEvaluator.h"

class ModifiesPEvaluator : public ModifiesUsesPEvaluator {
public:
    explicit ModifiesPEvaluator(PKBClient *pkb);

    /**
     * @param leftLabel A procedure name.
     * @param rightLabel A variable.
     * @return True if this procedure modifies this variable else false.
     */
    bool getIdentifierIdentifierRelation(const string &leftLabel, const string &rightLabel) override;

    /**
     * @param label A procedure name.
     * @return A set of variables that is being modified in this procedure.
     */
    unordered_set<string> getIdentifierSynonymRelation(const string &label) override;

    /**
     * @param label A procedure name.
     * @return True if set of variables modified in this procedure is not empty else false.
     */
    bool getIdentifierWildCardRelation(const string &label) override;

    /**
     * @param label A variable.
     * @return A set of procedures that modifies this variable.
     */
    unordered_set<string> getSynonymIdentifierRelation(const string &label) override;

    /**
     * @return A vector of procedure variable pair where procedure modifies this variable.
     */
    vector<pair<string, string>> getSynonymSynonymRelation() override;

    /**
     * @return A set of procedure that modifies variable
     */
    unordered_set<string> getSynonymWildCardRelation() override;

    /**
     * @param label A variable.
     * @return True if this variable is modified in any procedure else false.
     */
    bool getWildCardIdentifierRelation(const string &label) override;

    /**
     * @return True if any procedure modifies variable else false.
     */
    bool getWildCardWildCardRelation() override;
};
