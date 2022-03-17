#pragma once

#include "Evaluator.h"

class PatternEvaluator : public Evaluator {
public:
    Table* evaluate(PatternClause clause) override;
    Table* evaluateWildCardWildCard(QueryDeclaration patternSynonym);
    Table* evaluateIdentifierWildCard(QueryDeclaration patternSynonym, const ClauseVariable& leftVariable);

    virtual Table* evaluateFurther(QueryDeclaration patternSynonym, ClauseVariable& leftVariable,
                                   vector<PatternVariable>& rightPatternVariables) = 0;

    virtual unordered_set<string> getWildCardWildCardRelation() = 0;
    virtual unordered_set<string> getIdentifierWildCardRelation(const string& label) = 0;

    friend class IfWhilePatternEvaluator;
    friend class AssignPatternEvaluator;
};