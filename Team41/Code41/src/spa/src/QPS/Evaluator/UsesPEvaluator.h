#pragma once

#include "ModifiesUsesPEvaluator.h"

class UsesPEvaluator : public ModifiesUsesPEvaluator {
public:
    explicit UsesPEvaluator(PKBClient *pkb);

    /**
     * @param leftLabel A procedure name.
     * @param rightLabel A variable.
     * @return True if this procedure uses this variable else false.
     */
    bool getIdentifierIdentifierRelation(const string &leftLabel, const string &rightLabel) override;

    /**
     * @param label A procedure name.
     * @return A set of variables that is being used in this procedure.
     */
    unordered_set<string> getIdentifierSynonymRelation(const string &label) override;

    /**
     * @param label A procedure name.
     * @return True if set of variables used in this procedure is not empty else false.
     */
    bool getIdentifierWildCardRelation(const string &label) override;

    /**
     * @param label A variable.
     * @return A set of procedures that uses this variable.
     */
    unordered_set<string> getSynonymIdentifierRelation(const string &label) override;

    /**
     * @return A vector of procedure variable pair where procedure uses this variable.
     */
    vector<pair<string, string>> getSynonymSynonymRelation() override;

    /**
     * @return A set of procedure that uses variable
     */
    unordered_set<string> getSynonymWildCardRelation() override;

    /**
     * @param label A variable.
     * @return True if this variable is used in any procedure else false.
     */
    bool getWildCardIdentifierRelation(const string &label) override;

    /**
     * @return True if any procedure uses variable else false.
     */
    bool getWildCardWildCardRelation() override;
};