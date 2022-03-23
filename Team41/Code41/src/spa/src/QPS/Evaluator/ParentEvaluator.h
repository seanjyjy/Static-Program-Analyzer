#pragma once

#include "StmtStmtEvaluator.h"

class ParentEvaluator : public StmtStmtEvaluator {
public:
    explicit ParentEvaluator(PKBClient *pkb);

    /**
     * Construct a table that captures the Parent relation between two statement line.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Parent relation between two statement line.
     */
    Table *evaluateIntegerInteger(ClauseVariable left, ClauseVariable right) override;

    /**
     * Construct a table that captures the Parent relation between a statement line and a synonym.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Parent relation between a statement line and a synonym.
     */
    Table *evaluateIntegerSynonym(ClauseVariable left, ClauseVariable right) override;

    /**
     * Construct a table that captures the Parent relation between a statement line and a wildcard.
     *
     * @param left Clause variable.
     * @return A table that contains information based on the Parent relation between a statement line and a wildcard.
     */
    Table *evaluateIntegerWildCard(ClauseVariable left) override;

    /**
     * Construct a table that captures the Parent relation between a synonym and a statement line.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Parent relation between a synonym and a statement line.
     */
    Table *evaluateSynonymInteger(ClauseVariable left, ClauseVariable right) override;

    /**
     * Construct a table that captures the Parent relation between two synonym.
     *
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Parent relation between two synonym.
     */
    Table *evaluateSynonymSynonym(ClauseVariable left, ClauseVariable right) override;

    /**
     * Construct a table that captures the Parent relation between a synonym and a wildcard.
     *
     * @param left Clause variable.
     * @return A table that contains information based on the Parent relation between a synonym and a wildcard.
     */
    Table *evaluateSynonymWildCard(ClauseVariable left) override;

    /**
     * Construct a table that captures the Parent relation between a wildcard and a statement line.
     *
     * @param right Clause variable.
     * @return A table that contains information based on the Parent relation between a wildcard and a statement line.
     */
    Table *evaluateWildCardInteger(ClauseVariable right) override;

    /**
     * Construct a table that captures the Parent relation between a wildcard and a synonym.
     *
     * @param right Clause variable.
     * @return A table that contains information based on the Parent relation between a wildcard and a synonym.
     */
    Table *evaluateWildCardSynonym(ClauseVariable right) override;

    /**
     * Construct a table that captures the Parent relation between two wildcard.
     *
     * @return A table that contains information based on the Parent relation between two wildcard.
     */
    Table *evaluateWildCardWildCard() override;
};
