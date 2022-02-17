#pragma once

#include "Evaluator.h"

class ModifiesSEvaluator {
private:
    /**
     * Construct a table that captures the ModifiesS relation between a statement line and an identifier.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the ModifiesS relation between a statement line and
     * an identifier.
     */
    static Table* evaluateIntegerIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the ModifiesS relation between a statement line and a synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the ModifiesS relation between a statement line and a synonym.
     */
    static Table* evaluateIntegerSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the ModifiesS relation between a statement line and a wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @return A table that contains information based on the ModifiesS relation between a statement line and a wildcard.
     */
    static Table* evaluateIntegerWildCard(PKB* pkb, ClauseVariable left);

    /**
     * Construct a table that captures the ModifiesS relation between a synonym and an identifier.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the ModifiesS relation between a synonym and an identifier.
     */
    static Table* evaluateSynonymIdentifier(PKB* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the ModifiesS relation between two synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the ModifiesS relation between two synonym.
     */
    static Table* evaluateSynonymSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the ModifiesS relation between a synonym and a wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @return A table that contains information based on the ModifiesS relation between a synonym and a wildcard.
     */
    static Table* evaluateSynonymWildCard(PKB* pkb, ClauseVariable left);

public:
    /**
     * Depending on the QueryClause, choose a certain evaluate method for ModifiesS to evaluate this clause
     *
     * @param clause A QueryClause that contains the clause type, and two clause variable.
     * @param pkb A knowledge base based on the Source Program.
     * @return A table depending on the type of Query Clause.
     */
    static Table* evaluate(QueryClause clause, PKB *pkb);
};