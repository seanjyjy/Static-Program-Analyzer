#pragma once

#include "GenericClauseEvaluator.h"

class StmtStmtEvaluator : public GenericClauseEvaluator {
public:
    Table* evaluateClause(ClauseVariable left, ClauseVariable right) override;
    virtual Table* evaluateIntegerInteger(ClauseVariable left, ClauseVariable right) = 0;
    virtual Table* evaluateIntegerSynonym(ClauseVariable left, ClauseVariable right) = 0;
    virtual Table* evaluateIntegerWildCard(ClauseVariable left) = 0;
    virtual Table* evaluateSynonymInteger(ClauseVariable left, ClauseVariable right) = 0;
    virtual Table* evaluateSynonymSynonym(ClauseVariable left, ClauseVariable right) = 0;
    virtual Table* evaluateSynonymWildCard(ClauseVariable left) = 0;
    virtual Table* evaluateWildCardInteger(ClauseVariable right) = 0;
    virtual Table* evaluateWildCardSynonym(ClauseVariable right) = 0;
    virtual Table* evaluateWildCardWildCard() = 0;
};
