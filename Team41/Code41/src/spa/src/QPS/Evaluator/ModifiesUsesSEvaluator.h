#pragma once

#include "GenericClauseEvaluator.h"

class ModifiesUsesSEvaluator : public GenericClauseEvaluator {
public:
    explicit ModifiesUsesSEvaluator(PKBClient* pkb);

    Table* evaluateClause(ClauseVariable left, ClauseVariable right) override;
    Table* evaluateIntegerIdentifier(const ClauseVariable& left, const ClauseVariable& right);
    Table* evaluateIntegerSynonym(const ClauseVariable& left, ClauseVariable right);
    Table* evaluateIntegerWildCard(const ClauseVariable& left);
    Table* evaluateSynonymIdentifier(ClauseVariable& left, ClauseVariable& right);
    Table* evaluateSynonymSynonym(ClauseVariable left, ClauseVariable right);
    Table* evaluateSynonymWildCard(ClauseVariable& left);

    virtual Table* evaluateClauseFurther(ClauseVariable left, ClauseVariable right) = 0;

    virtual bool getIntegerIdentifierRelation(const string& leftLabel, const string& rightLabel) = 0;
    virtual unordered_set<string> getIntegerSynonymRelation(const string& label) = 0;
    virtual unordered_set<string> getIntegerWildCardRelation(const string& label) = 0;
    virtual unordered_set<string> getSynonymIdentifierRelation(const string& label) = 0;
    virtual vector<pair<string, string>> getSynonymSynonymRelation() = 0;
    virtual unordered_set<string> getSynonymWildCardRelation() = 0;

    friend class ModifiesSEvaluator;
    friend class UsesSEvaluator;
};
