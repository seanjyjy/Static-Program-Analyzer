#pragma once

#include "ProcProcEvaluator.h"

class CallsEvaluator : public ProcProcEvaluator {
public:
    explicit CallsEvaluator(PKBClient *pkb);

    /**
     * @param leftLabel A procedure name.
     * @param rightLabel A procedure name.
     * @return True if the left procedure calls directly the right procedure else false.
     */
    bool getIdentifierIdentifierRelation(const string &leftLabel, const string &rightLabel) override;

    /**
     * @param label A procedure name.
     * @return A set of procedures that is being called directly in this procedure.
     */
    unordered_set<string> getIdentifierSynonymRelation(const string &label) override;

    /**
     * @param label A procedure name.
     * @return True if this procedure calls directly any procedure else false.
     */
    bool getIdentifierWildCardRelation(const string &label) override;

    /**
     * @param label A procedure name.
     * @return A set of procedures that is calling directly this procedure.
     */
    unordered_set<string> getSynonymIdentifierRelation(const string &label) override;

    /**
     * @return A vector of procedure pairs there a procedure calls directly another procedure
     */
    vector<pair<string, string>> getSynonymSynonymRelation() override;

    /**
     * @return A set of procedures that calls directly some procedure
     */
    unordered_set<string> getSynonymWildCardRelation() override;

    /**
     * @param label A procedure name.
     * @return True if some procedure calls directly this procedure else false.
     */
    bool getWildCardIdentifierRelation(const string &label) override;

    /**
     * @return A set of procedures that is being called directly by some procedure
     */
    unordered_set<string> getWildCardSynonymRelation() override;

    /**
     * @return True if theres a procedure calling directly another procedure else false.
     */
    bool getWildCardWildCardRelation() override;
};
