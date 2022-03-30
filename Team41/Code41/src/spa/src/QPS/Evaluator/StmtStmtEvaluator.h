#pragma once

#include "GenericClauseEvaluator.h"

class StmtStmtEvaluator : public GenericClauseEvaluator {
protected:
    virtual Table *evaluateIntegerInteger(ClauseVariable &left, ClauseVariable &right) = 0;

    virtual Table *evaluateIntegerSynonym(ClauseVariable &left, ClauseVariable &right, bool canSimplify) = 0;

    virtual Table *evaluateIntegerWildCard(ClauseVariable &left) = 0;

    virtual Table *evaluateSynonymInteger(ClauseVariable &left, ClauseVariable &right, bool canSimplify) = 0;

    virtual Table *evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right, bool canSimplify) = 0;

    virtual Table *evaluateSynonymWildCard(ClauseVariable &left, bool canSimplify) = 0;

    virtual Table *evaluateWildCardInteger(ClauseVariable &right) = 0;

    virtual Table *evaluateWildCardSynonym(ClauseVariable &right, bool canSimplify) = 0;

    virtual Table *evaluateWildCardWildCard() = 0;

public:
    explicit StmtStmtEvaluator(PKBClient *pkb);

    Table *evaluateClause(ClauseVariable &left, ClauseVariable &right, bool canSimplify) override;

    friend class FollowsEvaluator;

    friend class FollowsTEvaluator;

    friend class ParentEvaluator;

    friend class ParentTEvaluator;

    friend class NextEvaluator;

    friend class NextTEvaluator;
};
