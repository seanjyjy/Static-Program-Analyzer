#pragma once

#include "Evaluator.h"

class FollowsTEvaluator {
private:
    /**
     * Construct a table that captures the follow* relation between two statement line.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the follow* relation between two statement line.
     */
    static Table* evaluateIntegerInteger(PKB* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the follow* relation between a statement line and a synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the follow* relation between a statement line and a synonym.
     */
    static Table* evaluateIntegerSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the follow* relation between a statement line and a synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @return A table that contains information based on the follow* relation between a statement line and a synonym.
     */
    static Table* evaluateIntegerWildCard(PKB* pkb, ClauseVariable left);

    /**
     * Construct a table that captures the follow* relation between a statement line and a synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the follow* relation between a statement line and a synonym.
     */
    static Table* evaluateSynonymInteger(PKB* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the follow* relation between two synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @param right Clause variable.
     * @return A table that contains information based on the follow* relation between two synonym.
     */
    static Table* evaluateSynonymSynonym(PKB* pkb, ClauseVariable left, ClauseVariable right);

    /**
     * Construct a table that captures the follow* relation between a synonym and a wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param left Clause variable.
     * @return A table that contains information based on the follow* relation between a synonym and a wildcard.
     */
    static Table* evaluateSynonymWildCard(PKB* pkb, ClauseVariable left);

    /**
     * Construct a table that captures the follow* relation between a wildcard and a statement line.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param right Clause variable.
     * @return A table that contains information based on the follow* relation between a wildcard and a statement line.
     */
    static Table* evaluateWildCardInteger(PKB* pkb, ClauseVariable right);

    /**
     * Construct a table that captures the follow* relation between a wildcard and a synonym.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param right Clause variable.
     * @return A table that contains information based on the follow* relation between a wildcard and a synonym.
     */
    static Table* evaluateWildCardSynonym(PKB* pkb, ClauseVariable right);

    /**
     * Construct a table that captures the follow* relation between two wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @return A table that contains information based on the follow* relation between two wildcard.
     */
    static Table* evaluateWildCardWildCard(PKB* pkb);
public:
    /**
     * Depending on the QueryClause, choose a certain evaluate method for Follow* to evaluate this clause
     *
     * @param clause A QueryClause that contains the clause type, and two clause variable.
     * @param pkb A knowledge base based on the Source Program.
     * @return A table depending on the type of Query Clause.
     */
    static Table* evaluate(QueryClause clause, PKB *pkb);
};