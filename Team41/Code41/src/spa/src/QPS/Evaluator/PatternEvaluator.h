#pragma once

#include "Evaluator.h"
#include "Common/TNode.h"
#include "../PatternClause.h"

class PatternEvaluator {
private:
    static Table *evaluateWildCardFullPattern(PKB *pkb, QueryDeclaration patternSyn, PatternVariable right);
    static Table *evaluateWildCardSubPattern(PKB *pkb, QueryDeclaration patternSyn, PatternVariable right);
    static Table *evaluateWildCardWildCard(PKB *pkb, QueryDeclaration patternSyn);

    static Table *evaluateIdentifierFullPattern(PKB *pkb, QueryDeclaration patternSyn,
                                                ClauseVariable left, PatternVariable right);
    static Table *evaluateIdentifierSubPattern(PKB *pkb, QueryDeclaration patternSyn,
                                               ClauseVariable left, PatternVariable right);
    static Table *evaluateIdentifierWildCard(PKB *pkb, QueryDeclaration patternSyn, ClauseVariable left);

    static Table *evaluateSynonymFullPattern(PKB *pkb, QueryDeclaration patternSyn,
                                             ClauseVariable left, PatternVariable right);
    static Table *evaluateSynonymSubPattern(PKB *pkb, QueryDeclaration patternSyn,
                                            ClauseVariable left, PatternVariable right);
    static Table *evaluateSynonymWildCard(PKB *pkb, QueryDeclaration patternSyn, ClauseVariable left);

public:
    static Table *evaluate(PatternClause clause, PKB *pkb);
};