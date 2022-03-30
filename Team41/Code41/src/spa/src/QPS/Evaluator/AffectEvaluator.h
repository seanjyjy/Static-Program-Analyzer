#pragma once

#include "GenericClauseEvaluator.h"

class AffectEvaluator : public GenericClauseEvaluator {
private:
    Table *evaluateIntegerInteger(const ClauseVariable &left, const ClauseVariable &right);

    Table *evaluateIntegerSynonym(const ClauseVariable &left, ClauseVariable &right, bool canSimplify);

    Table *evaluateIntegerWildCard(const ClauseVariable &left);

    Table *evaluateSynonymInteger(ClauseVariable &left, const ClauseVariable &right, bool canSimplify);

    Table *evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right, bool canSimplify);

    Table *evaluateSynonymWildCard(ClauseVariable &left, bool canSimplify);

    Table *evaluateWildCardInteger(const ClauseVariable &right);

    Table *evaluateWildCardSynonym(ClauseVariable &right, bool canSimplify);

    Table *evaluateWildCardWildCard();

protected:
    AffectsKBAdapter *affectsKBAdapter;

    bool isValidAssignAssign(ClauseVariable &left, ClauseVariable &right);

    bool isValidAssignStmt(ClauseVariable &variable);

    bool isValidAssignSyn(ClauseVariable &left);

    bool isValidAssignSynSyn(ClauseVariable &left, ClauseVariable &right);

    virtual bool getIntegerIntegerRelation(const string &left, const string &right) = 0;

    virtual unordered_set<string> getIntegerSynonymRelation(const string &label) = 0;

    virtual unordered_set<string> getSynonymIntegerRelation(const string &label) = 0;

    virtual vector<pair<string, string>> getSynonymSynonymRelation() = 0;

    virtual unordered_set<string> getSynonymWildCardRelation() = 0;

    virtual unordered_set<string> getWildCardSynonymRelation() = 0;

    bool getIntegerWildCardRelation(const string &label);

    bool getWildCardIntegerRelation(const string &label);

    bool getWildCardWildCardRelation();

public:
    explicit AffectEvaluator(PKBClient *pkb, AffectsKBAdapter *adapter);

    Table *evaluateClause(ClauseVariable &left, ClauseVariable &right, bool canSimplify) override;

    friend class AffectsEvaluator;
};
