#pragma once

#include "GenericClauseEvaluator.h"

class AffectEvaluator : public GenericClauseEvaluator {
private:
    Table* evaluateIntegerInteger(const ClauseVariable& left, const ClauseVariable& right);
    Table* evaluateIntegerSynonym(const ClauseVariable& left, ClauseVariable right);
    Table* evaluateIntegerWildCard(const ClauseVariable& left);
    Table* evaluateSynonymInteger(ClauseVariable left, const ClauseVariable& right);
    Table* evaluateSynonymSynonym(ClauseVariable left, ClauseVariable right);
    Table* evaluateSynonymWildCard(ClauseVariable left);
    Table* evaluateWildCardInteger(const ClauseVariable& right);
    Table* evaluateWildCardSynonym(ClauseVariable right);
    Table* evaluateWildCardWildCard();
protected:
    bool isValidAssignAssign(const string& stmt1, const string& stmt2);
    bool isValidAssignStmt(const string& stmt);
    bool isValidAssignSyn(ClauseVariable &left);
    bool isValidAssignSynSyn(ClauseVariable &left, ClauseVariable &right);

    virtual bool getIntegerIntegerRelation(const string& left, const string& right) = 0;
    virtual unordered_set<string> getIntegerSynonymRelation(const string& label) = 0;
    virtual unordered_set<string> getIntegerWildCardRelation(const string& label) = 0;
    virtual unordered_set<string> getSynonymIntegerRelation(const string& label) = 0;
    virtual vector<pair<string, string>> getSynonymSynonymRelation() = 0;
    virtual unordered_set<string> getSynonymWildCardRelation() = 0;
    virtual unordered_set<string> getWildCardIntegerRelation(const string& label) = 0;
    virtual unordered_set<string> getWildCardSynonymRelation() = 0;
    virtual vector<pair<string, string>> getWildCardWildCardRelation() = 0;
public:
    explicit AffectEvaluator(PKBClient* pkb);

    Table* evaluateClause(ClauseVariable left, ClauseVariable right) override;

    friend class AffectsEvaluator;
};
