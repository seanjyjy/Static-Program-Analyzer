#pragma once

#include "ProcProcEvaluator.h"

class CallsTEvaluator : public ProcProcEvaluator {
public:
    explicit CallsTEvaluator(PKBClient *pkb);

    /**
     * @param leftLabel A procedure name.
     * @param rightLabel A procedure name.
     * @return True if the left procedure calls directly or indirectly the right procedure else false.
     */
    bool getIdentifierIdentifierRelation(const string &leftLabel, const string &rightLabel) override;

    /**
     * @param label A procedure name.
     * @return A set of procedures that is being called directly or indirectly in this procedure.
     */
    unordered_set<string> getIdentifierSynonymRelation(const string &label) override;

    /**
     * @param label A procedure name.
     * @return True if this procedure calls directly or indirectly any procedure else false.
     */
    bool getIdentifierWildCardRelation(const string &label) override;

    /**
     * @param label A procedure name.
     * @return A set of procedures that is calling directly or indirectly this procedure.
     */
    unordered_set<string> getSynonymIdentifierRelation(const string &label) override;

    /**
     * @return A vector of procedure pairs there a procedure calls directly or indirectly another procedure
     */
    vector<pair<string, string>> getSynonymSynonymRelation() override;

    /**
     * @return A set of procedures that calls directly or indirectly some procedure
     */
    unordered_set<string> getSynonymWildCardRelation() override;

    /**
     * @param label A procedure name.
     * @return True if some procedure calls directly or indirectly this procedure else false.
     */
    bool getWildCardIdentifierRelation(const string &label) override;

    /**
     * @return A set of procedures that is being called directly or indirectly by some procedure
     */
    unordered_set<string> getWildCardSynonymRelation() override;

    /**
     * @return True if theres a procedure calling directly or indirectly another procedure else false.
     */
    bool getWildCardWildCardRelation() override;
};
