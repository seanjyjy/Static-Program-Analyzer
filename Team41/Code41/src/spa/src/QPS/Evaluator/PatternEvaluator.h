#pragma once

#include "Evaluator.h"

class PatternEvaluator : public Evaluator {
public:
    explicit PatternEvaluator(PKBClient *pkb);
    Table* evaluate(PatternClause clause) override;
    // TODO REMOVE THIS IN THE FUTURE
    Table* evaluate(QueryClause clause) override;
    Table* evaluateWildCardWildCard(QueryDeclaration patternSynonym);

    virtual Table* evaluateFurther(QueryDeclaration patternSynonym, ClauseVariable& leftVariable,
                                   vector<PatternVariable>& rightPatternVariables) = 0;

    virtual unordered_set<string> getWildCardWildCardRelation() = 0;

    friend class IfWhilePatternEvaluator;
    friend class AssignPatternEvaluator;
};