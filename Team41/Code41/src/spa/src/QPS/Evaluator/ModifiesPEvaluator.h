#pragma once

#include "Evaluator.h"

class ModifiesPEvaluator {
private:
    /**
     * Construct a table that captures the ModifiesP relation between two identifiers.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the ModifiesP relation between two identifiers.
     */
    static Table* evaluateIdentifierIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the ModifiesP relation between an identifier and a synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the ModifiesP relation between an identifier and a synonym.
     */
    static Table* evaluateIdentifierSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the ModifiesP relation between an identifier and a wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @return A table that contains information based on the ModifiesP relation between an identifier and a wildcard.
     */
    static Table* evaluateIdentifierWildCard(PKB* pkb, ClauseVariable left);

    /**
     * Construct a table that captures the ModifiesP relation between a synonym and an identifier.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the ModifiesP relation between a synonym and an identifier.
     */
    static Table* evaluateSynonymIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the ModifiesP relation between two synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the ModifiesP relation between two synonym.
     */
    static Table* evaluateSynonymSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the ModifiesP relation between a synonym and a wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @return A table that contains information based on the ModifiesP relation between a synonym and a wildcard.
     */
    static Table* evaluateSynonymWildCard(PKB* pkb, ClauseVariable left);
public:
    /**
     * Depending on the QueryClause, choose a certain evaluate method for ModifiesP to evaluate this clause
     *
     * @param clause A QueryClause that contains the clause type, and two clause variable.
     * @param pkb A knowledge base based on the Source Program.
     * @return A table depending on the type of Query Clause.
     */
    static Table* evaluate(QueryClause clause, PKB *pkb);
};
