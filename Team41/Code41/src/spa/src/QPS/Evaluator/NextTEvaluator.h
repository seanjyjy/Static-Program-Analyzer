#pragma once

#include "Evaluator.h"

class NextTEvaluator {
private:
    /**
     * Construct a table that captures the Next* relation between two statement line.
     *
     * @param nextKBAdapter An adapter that allows communication between cache and PKB(@see PKB)
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Next* relation between two statement line.
     */
    static Table* evaluateIntegerInteger(NextKBAdapter* nextKBAdapter, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the Next* relation between a statement line and a synonym.
     *
     * @param nextKBAdapter An adapter that allows communication between cache and PKB(@see PKB)
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Next* relation between a statement line and a synonym.
     */
    static Table* evaluateIntegerSynonym(NextKBAdapter* nextKBAdapter, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the Next* relation between a statement line and a wildcard.
     *
     * @param nextKBAdapter An adapter that allows communication between cache and PKB(@see PKB)
     * @param left Clause variable.
     * @return A table that contains information based on the Next* relation between a statement line and a wildcard.
     */
    static Table* evaluateIntegerWildCard(NextKBAdapter* nextKBAdapter, ClauseVariable left);

    /**
     * Construct a table that captures the Next* relation between a synonym and a statement line.
     *
     * @param nextKBAdapter An adapter that allows communication between cache and PKB(@see PKB)
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Next* relation between a synonym and a statement line.
     */
    static Table* evaluateSynonymInteger(NextKBAdapter* nextKBAdapter, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the Next* relation between two synonym.
     *
     * @param nextKBAdapter An adapter that allows communication between cache and PKB(@see PKB)
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Next* relation between two synonym.
     */
    static Table* evaluateSynonymSynonym(NextKBAdapter* nextKBAdapter, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the Next* relation between a synonym and a wildcard.
     *
     * @param nextKBAdapter An adapter that allows communication between cache and PKB(@see PKB)
     * @param left Clause variable.
     * @return A table that contains information based on the Next* relation between a synonym and a wildcard.
     */
    static Table* evaluateSynonymWildCard(NextKBAdapter* nextKBAdapter, ClauseVariable left);

    /**
     * Construct a table that captures the Next* relation between a wildcard and a statement line.
     *
     * @param nextKBAdapter An adapter that allows communication between cache and PKB(@see PKB)
     * @param right Clause variable.
     * @return A table that contains information based on the Next* relation between a wildcard and a statement line.
     */
    static Table* evaluateWildCardInteger(NextKBAdapter* nextKBAdapter, ClauseVariable right);

    /**
     * Construct a table that captures the Next* relation between a wildcard and a synonym.
     *
     * @param nextKBAdapter An adapter that allows communication between cache and PKB(@see PKB)
     * @param right Clause variable.
     * @return A table that contains information based on the Next* relation between a wildcard and a synonym.
     */
    static Table* evaluateWildCardSynonym(NextKBAdapter* nextKBAdapter, ClauseVariable right);

    /**
     * Construct a table that captures the Next* relation between two wildcard.
     *
     * @param nextKBAdapter An adapter that allows communication between cache and PKB(@see PKB)
     * @return A table that contains information based on the Next* relation between two wildcard.
     */
    static Table* evaluateWildCardWildCard(NextKBAdapter* nextKBAdapter);
public:
    /**
     * Depending on the QueryClause, choose a certain evaluate method for Next* to evaluate this clause
     *
     * @param clause A QueryClause that contains the clause type, and two clause variable.
     * @param nextKBAdapter An adapter that allows communication between cache and PKB(@see PKB)
     * @return A table depending on the type of Query Clause.
     */
    static Table* evaluate(QueryClause clause, NextKBAdapter* nextKBAdapter);
};