#pragma once

#include "Evaluator.h"

class CallsEvaluator {
private:
    /**
     * Construct a table that captures the Calls relation between two identifiers.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Calls relation between two identifiers.
     */
    static Table* evaluateIdentifierIdentifier(PKBClient *pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the Calls relation between an identifier and a synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Calls relation between an identifier and a synonym.
     */
    static Table* evaluateIdentifierSynonym(PKBClient* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the Calls relation between an identifier and a wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @return A table that contains information based on the Calls relation between an identifier and a wildcard.
     */
    static Table* evaluateIdentifierWildCard(PKBClient* pkb, ClauseVariable left);

    /**
     * Construct a table that captures the Calls relation between a synonym and an identifier.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Calls relation between a synonym and an identifier.
     */
    static Table* evaluateSynonymIdentifier(PKBClient* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the Calls relation between two synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the Calls relation between two synonym.
     */
    static Table* evaluateSynonymSynonym(PKBClient* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the Calls relation between a synonym and a wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @return A table that contains information based on the Calls relation between a synonym and a wildcard.
     */
    static Table* evaluateSynonymWildCard(PKBClient* pkb, ClauseVariable left);

    /**
     * Construct a table that captures the Calls relation between a wildcard and an identifier.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param right Clause variable.
     * @return A table that contains information based on the Calls relation between a wildcard and an identifier.
     */
    static Table* evaluateWildCardIdentifier(PKBClient* pkb, ClauseVariable right);

    /**
     * Construct a table that captures the Calls relation between a wildcard and a synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param right Clause variable.
     * @return A table that contains information based on the Calls relation between a wildcard and a synonym.
     */
    static Table* evaluateWildCardSynonym(PKBClient* pkb, ClauseVariable right);

    /**
     * Construct a table that captures the Calls relation between two wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param right Clause variable.
     * @return A table that contains information based on the Calls relation between two wildcard.
     */
    static Table* evaluateWildCardWildCard(PKBClient* pkb);
public:
    /**
     * Depending on the QueryClause, choose a certain evaluate method for Call to evaluate this clause
     *
     * @param clause A QueryClause that contains the clause type, and two clause variable.
     * @param pkb A knowledge base based on the Source Program.
     * @return A table depending on the type of Query Clause.
     */
    static Table* evaluate(QueryClause clause, PKBClient *pkb);
};
