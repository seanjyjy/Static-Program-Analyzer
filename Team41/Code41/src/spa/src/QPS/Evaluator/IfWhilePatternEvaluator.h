#pragma once

#include "PatternEvaluator.h"

class IfWhilePatternEvaluator : public PatternEvaluator {
public:
    explicit IfWhilePatternEvaluator(PKBClient* pkb);
    Table* evaluateFurther(QueryDeclaration patternSynonym, ClauseVariable& leftVariable,
                           vector<PatternVariable>& rightPatternVariables) override;

    Table* evaluateSynonymWildCard(QueryDeclaration& patternSyn, ClauseVariable& left);
    Table* evaluateIdentifierWildCard(QueryDeclaration patternSynonym, const ClauseVariable& leftVariable);

    virtual vector<pair<string, string>> getSynonymWildCardRelation() = 0;
    virtual unordered_set<string> getIdentifierWildCardRelation(const string& label) = 0;

    friend class IfPatternEvaluator;
    friend class WhilePatternEvaluator;
};
