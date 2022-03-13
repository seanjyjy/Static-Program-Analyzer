#pragma once

#include "Evaluator.h"

class NextTEvaluator {
private:
    /**
     * Construct a table that captures the Next* relation between two statement line.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Next* relation between two statement line.
     */
    static Table* evaluateIntegerInteger(PKBClient* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the Next* relation between a statement line and a synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Next* relation between a statement line and a synonym.
     */
    static Table* evaluateIntegerSynonym(PKBClient* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the Next* relation between a statement line and a wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @return A table that contains information based on the Next* relation between a statement line and a wildcard.
     */
    static Table* evaluateIntegerWildCard(PKBClient* pkb, ClauseVariable left);

    /**
     * Construct a table that captures the Next* relation between a synonym and a statement line.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Next* relation between a synonym and a statement line.
     */
    static Table* evaluateSynonymInteger(PKBClient* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the Next* relation between two synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Next* relation between two synonym.
     */
    static Table* evaluateSynonymSynonym(PKBClient* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the Next* relation between a synonym and a wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @return A table that contains information based on the Next* relation between a synonym and a wildcard.
     */
    static Table* evaluateSynonymWildCard(PKBClient* pkb, ClauseVariable left);

    /**
     * Construct a table that captures the Next* relation between a wildcard and a statement line.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param right Clause variable.
     * @return A table that contains information based on the Next* relation between a wildcard and a statement line.
     */
    static Table* evaluateWildCardInteger(PKBClient* pkb, ClauseVariable right);

    /**
     * Construct a table that captures the Next* relation between a wildcard and a synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param right Clause variable.
     * @return A table that contains information based on the Next* relation between a wildcard and a synonym.
     */
    static Table* evaluateWildCardSynonym(PKBClient* pkb, ClauseVariable right);

    /**
     * Construct a table that captures the Next* relation between two wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @return A table that contains information based on the Next* relation between two wildcard.
     */
    static Table* evaluateWildCardWildCard(PKBClient* pkb);
public:
    /**
     * Depending on the QueryClause, choose a certain evaluate method for Next* to evaluate this clause
     *
     * @param clause A QueryClause that contains the clause type, and two clause variable.
     * @param pkb A knowledge base based on the Source Program.
     * @return A table depending on the type of Query Clause.
     */
    static Table* evaluate(QueryClause clause, PKBClient *pkb);
};