#pragma once

#include "GenericClauseEvaluator.h"

class AffectEvaluator : public GenericClauseEvaluator {
private:
    unordered_set<string> assignStmts;
protected:
    virtual Table* evaluateIntegerInteger(ClauseVariable left, ClauseVariable right) = 0;
    virtual Table* evaluateIntegerSynonym(ClauseVariable left, ClauseVariable right) = 0;
    virtual Table* evaluateIntegerWildCard(ClauseVariable left) = 0;
    virtual Table* evaluateSynonymInteger(ClauseVariable left, ClauseVariable right) = 0;
    virtual Table* evaluateSynonymSynonym(ClauseVariable left, ClauseVariable right) = 0;
    virtual Table* evaluateSynonymWildCard(ClauseVariable left) = 0;
    virtual Table* evaluateWildCardInteger(ClauseVariable right) = 0;
    virtual Table* evaluateWildCardSynonym(ClauseVariable right) = 0;
    virtual Table* evaluateWildCardWildCard() = 0;

    bool isValidAssignAssign(const string& stmt1, const string& stmt2);
    bool isValidAssignStmt(const string& stmt);
    bool isValidAssignSyn(ClauseVariable &left);
    bool isValidAssignSynSyn(ClauseVariable &left, ClauseVariable &right);
public:
    explicit AffectEvaluator(PKBClient* pkb);

    Table* evaluateClause(ClauseVariable left, ClauseVariable right) override;
};
