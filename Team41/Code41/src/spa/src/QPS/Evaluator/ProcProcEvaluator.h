#pragma once

#include "GenericClauseEvaluator.h"

class ProcProcEvaluator : public GenericClauseEvaluator {
    Table* evaluateClause(ClauseVariable left, ClauseVariable right) override;
    Table* evaluateIdentifierIdentifier(ClauseVariable left, ClauseVariable right);
    Table* evaluateIdentifierSynonym(ClauseVariable left, ClauseVariable right);
    Table* evaluateIdentifierWildCard(ClauseVariable left);
    Table* evaluateSynonymIdentifier(ClauseVariable left, ClauseVariable right);
    Table* evaluateSynonymSynonym(ClauseVariable left, ClauseVariable right);
    Table* evaluateSynonymWildCard(ClauseVariable left);
    Table* evaluateWildCardIdentifier(ClauseVariable right);
    Table* evaluateWildCardSynonym(ClauseVariable right);
    Table* evaluateWildCardWildCard();

    virtual bool getIdentifierIdentifierRelation(const string& leftLabel, const string& rightLabel) = 0;
    virtual unordered_set<string> getIdentifierSynonymRelation(const string& label) = 0;
    virtual unordered_set<string> getIdentifierWildCardRelation(const string& label) = 0;
    virtual unordered_set<string> getSynonymIdentifierRelation(const string& label) = 0;
    virtual vector<pair<string, string>> getSynonymSynonymRelation() = 0;
    virtual unordered_set<string> getSynonymWildCardRelation() = 0;
    virtual unordered_set<string> getWildCardIdentifierRelation(const string& label) = 0;
    virtual unordered_set<string> getWildCardSynonymRelation() = 0;
    virtual vector<pair<string, string>> getWildCardWildCardRelation() = 0;
};