#pragma once

#include "PatternEvaluator.h"

class IfWhilePatternEvaluator : PatternEvaluator {
public:
    explicit IfWhilePatternEvaluator(PKBClient* pkb);
    Table* evaluateFurther(QueryDeclaration patternSynonym, ClauseVariable& leftVariable,
                           vector<PatternVariable>& rightPatternVariables) override;

    Table* evaluateSynonymWildCard(QueryDeclaration& patternSyn, ClauseVariable& left);

    virtual vector<pair<string, string>> getSynonymWildCardRelation() = 0;

    friend class IfPatternEvaluator;
    friend class WhilePatternEvaluator;
};
