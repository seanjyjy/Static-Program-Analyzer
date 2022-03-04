#pragma once

#include "Evaluator.h"

class CallsEvaluator {
private:
    static Table* evaluateIdentifierIdentifier(PKBClient *pkb, ClauseVariable left, ClauseVariable right);

    static Table* evaluateIdentifierSynonym(PKBClient* pkb, ClauseVariable left, ClauseVariable right);

    static Table* evaluateIdentifierWildCard(PKBClient* pkb, ClauseVariable left);

    static Table* evaluateSynonymIdentifier(PKBClient* pkb, ClauseVariable left, ClauseVariable right);

    static Table* evaluateSynonymSynonym(PKBClient* pkb, ClauseVariable left, ClauseVariable right);

    static Table* evaluateSynonymWildCard(PKBClient* pkb, ClauseVariable left);

    static Table* evaluateWildCardIdentifier(PKBClient* pkb, ClauseVariable right);

    static Table* evaluateWildCardSynonym(PKBClient* pkb, ClauseVariable right);

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
