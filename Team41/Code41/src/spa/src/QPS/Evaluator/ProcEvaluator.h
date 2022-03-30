#pragma once

#include "GenericClauseEvaluator.h"

class ProcEvaluator : public GenericClauseEvaluator {
public:
    explicit ProcEvaluator(PKBClient *pkb);

    Table *evaluateIdentifierIdentifier(const ClauseVariable &left, const ClauseVariable &right);

    Table *evaluateIdentifierSynonym(const ClauseVariable &left, ClauseVariable &right, bool canSimplify);

    Table *evaluateIdentifierWildCard(const ClauseVariable &left);

    Table *evaluateSynonymIdentifier(ClauseVariable &left, const ClauseVariable &right, bool canSimplify);

    Table *evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right, bool canSimplify);

    Table *evaluateSynonymWildCard(ClauseVariable &left, bool canSimplify);

    Table *evaluateWildCardIdentifier(const ClauseVariable &right);

    Table *evaluateWildCardWildCard();

    virtual bool getIdentifierIdentifierRelation(const string &leftLabel, const string &rightLabel) = 0;

    virtual unordered_set<string> getIdentifierSynonymRelation(const string &label) = 0;

    virtual bool getIdentifierWildCardRelation(const string &label) = 0;

    virtual unordered_set<string> getSynonymIdentifierRelation(const string &label) = 0;

    virtual vector<pair<string, string>> getSynonymSynonymRelation() = 0;

    virtual unordered_set<string> getSynonymWildCardRelation() = 0;

    virtual bool getWildCardIdentifierRelation(const string &label) = 0;

    virtual bool getWildCardWildCardRelation() = 0;

    friend class ProcProcEvaluator;

    friend class ModifiesUsesPEvaluator;
};