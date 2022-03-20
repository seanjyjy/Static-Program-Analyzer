#pragma once

#include "GenericClauseEvaluator.h"

class ProcEvaluator : public GenericClauseEvaluator {
public:
    explicit ProcEvaluator(PKBClient* pkb);

    Table* evaluateIdentifierIdentifier(const ClauseVariable& left, const ClauseVariable& right);
    Table* evaluateIdentifierSynonym(const ClauseVariable& left, ClauseVariable& right);
    Table* evaluateIdentifierWildCard(const ClauseVariable& left);
    Table* evaluateSynonymIdentifier(ClauseVariable& left, const ClauseVariable& right);
    Table* evaluateSynonymSynonym(ClauseVariable& left, ClauseVariable& right);
    Table* evaluateSynonymWildCard(ClauseVariable& left);

    virtual bool getIdentifierIdentifierRelation(const string& leftLabel, const string& rightLabel) = 0;
    virtual unordered_set<string> getIdentifierSynonymRelation(const string& label) = 0;
    virtual unordered_set<string> getIdentifierWildCardRelation(const string& label) = 0;
    virtual unordered_set<string> getSynonymIdentifierRelation(const string& label) = 0;
    virtual vector<pair<string, string>> getSynonymSynonymRelation() = 0;
    virtual unordered_set<string> getSynonymWildCardRelation() = 0;

    friend class ProcProcEvaluator;
    friend class ModifiesUsesPEvaluator;
};