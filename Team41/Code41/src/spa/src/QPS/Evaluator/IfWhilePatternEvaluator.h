#pragma once

#include "PatternEvaluator.h"

class IfWhilePatternEvaluator : public PatternEvaluator {
public:
    explicit IfWhilePatternEvaluator(PKBClient *pkb);

    Table *evaluateFurther(QueryDeclaration &patternSynonym, ClauseVariable &leftVariable,
                           vector<PatternVariable> &rightPatternVariables, bool canSimplify) override;

    Table *evaluateSynonymWildCard(QueryDeclaration &patternSyn, ClauseVariable &left, bool canSimplify);

    Table *evaluateIdentifierWildCard(QueryDeclaration &patternSynonym, const ClauseVariable &leftVariable, bool canSimplify);

    virtual vector<pair<string, string>> getSynonymWildCardRelation() = 0;

    virtual unordered_set<string> getIdentifierWildCardRelation(const string &label) = 0;

    friend class IfPatternEvaluator;

    friend class WhilePatternEvaluator;
};