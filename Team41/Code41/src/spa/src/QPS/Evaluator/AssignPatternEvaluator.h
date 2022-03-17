#pragma once

#include "PatternEvaluator.h"
#include "Common/TNode.h"
#include "../PatternClause.h"

class AssignPatternEvaluator : public PatternEvaluator {
public:
    explicit AssignPatternEvaluator(PKBClient* pkb);

    Table* evaluateFurther(QueryDeclaration patternSynonym, ClauseVariable& leftVariable,
                                   vector<PatternVariable>& rightPatternVariables) override;

    unordered_set<string> getWildCardWildCardRelation() override;
    unordered_set<string> getIdentifierWildCardRelation(const string& label) override;

    /**
     * Construct a table that captures the Pattern relation between a wildcard and a full pattern.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param patternSyn QueryDeclaration that contains the pattern synonym.
     * @param right Pattern variable.
     * @return A table that contains information based on the Pattern relation between a wildcard and a full pattern.
     */
    Table *evaluateWildCardFullPattern(QueryDeclaration patternSyn, PatternVariable right);

    /**
     * Construct a table that captures the Pattern relation between a wildcard and a sub pattern.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param patternSyn QueryDeclaration that contains the pattern synonym.
     * @param right Pattern variable.
     * @return A table that contains information based on the Pattern relation between a wildcard and a sub pattern.
     */
    Table *evaluateWildCardSubPattern(QueryDeclaration patternSyn, PatternVariable right);

    /**
     * Construct a table that captures the Pattern relation between an identifier and a full pattern.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param patternSyn QueryDeclaration that contains the pattern synonym.
     * @param right Pattern variable.
     * @return A table that contains information based on the Pattern relation between an identifier and a full pattern.
     */
    Table *evaluateIdentifierFullPattern(QueryDeclaration patternSyn, ClauseVariable left, PatternVariable right);

    /**
     * Construct a table that captures the Pattern relation between an identifier and a sub pattern.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param patternSyn QueryDeclaration that contains the pattern synonym.
     * @param right Pattern variable.
     * @return A table that contains information based on the Pattern relation between an identifier and a sub pattern.
     */
    Table *evaluateIdentifierSubPattern(QueryDeclaration patternSyn, ClauseVariable left, PatternVariable right);

    /**
     * Construct a table that captures the Pattern relation between a synonym and a full pattern.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param patternSyn QueryDeclaration that contains the pattern synonym.
     * @param right Pattern variable.
     * @return A table that contains information based on the Pattern relation between a synonym and a full pattern.
     */
    Table *evaluateSynonymFullPattern(QueryDeclaration patternSyn, ClauseVariable left, PatternVariable right);

    /**
     * Construct a table that captures the Pattern relation between a synonym and a sub pattern.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param patternSyn QueryDeclaration that contains the pattern synonym.
     * @param right Pattern variable.
     * @return A table that contains information based on the Pattern relation between a synonym and a sub pattern.
     */
    Table *evaluateSynonymSubPattern(QueryDeclaration patternSyn, ClauseVariable left, PatternVariable right);

    /**
     * Construct a table that captures the Pattern relation between a synonym and a wildcard.
     *
     * @param pkb A knowledge base based on the Source Program.
     * @param patternSyn QueryDeclaration that contains the pattern synonym.
     * @param right Pattern variable.
     * @return A table that contains information based on the Pattern relation between a synonym and a wildcard.
     */
    Table *evaluateSynonymWildCard(QueryDeclaration patternSyn, ClauseVariable left);
};