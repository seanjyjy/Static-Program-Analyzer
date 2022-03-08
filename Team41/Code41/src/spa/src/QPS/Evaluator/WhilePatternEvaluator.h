#pragma once

#include "Evaluator.h"
#include "Common/TNode.h"
#include "../PatternClause.h"

class WhilePatternEvaluator {
private:
    /**
     * Construct a table that captures the Pattern relation between two wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param patternSyn QueryDeclaration that contains the pattern synonym.
     * @return A table that contains information based on the Pattern relation between two wildcard.
     */
    static Table *evaluateWildCardWildCard(PKBClient *pkb, QueryDeclaration patternSyn);

    /**
     * Construct a table that captures the Pattern relation between an identifier and a wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param patternSyn QueryDeclaration that contains the pattern synonym.
     * @param left Pattern variable.
     * @return A table that contains information based on the Pattern relation between an identifier and a wildcard.
     */
    static Table *evaluateIdentifierWildCard(PKBClient *pkb, QueryDeclaration patternSyn, ClauseVariable left);

    /**
     * Construct a table that captures the Pattern relation between a synonym and a wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param patternSyn QueryDeclaration that contains the pattern synonym.
     * @param left Pattern variable.
     * @return A table that contains information based on the Pattern relation between a synonym and a wildcard.
     */
    static Table *evaluateSynonymWildCard(PKBClient *pkb, QueryDeclaration patternSyn, ClauseVariable left);

public:
    /**
     * Depending on the QueryClause, choose a certain evaluate method for Pattern to evaluate this clause
     *
     * @param clause A QueryClause that contains the clause type, and two clause variable.
     * @param pkb A knowledge base based on the Source Program.
     * @return A table depending on the type of Query Clause.
     */
    static Table *evaluate(PatternClause clause, PKBClient *pkb);
};