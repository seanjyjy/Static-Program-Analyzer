#pragma once

#include "IfWhilePatternEvaluator.h"
#include "Common/TNode.h"
#include "../PatternClause.h"

class WhilePatternEvaluator : public IfWhilePatternEvaluator {
public:
    explicit WhilePatternEvaluator(PKBClient *pkb);

    /**
     * @return A set of stmts that fulfils the wildcard wildcard relation of while pattern.
     */
    unordered_set<string> getWildCardWildCardRelation() override;

    /**
     * @return A set of stmts that fulfils the identifier wildcard relation of while pattern.
     */
    unordered_set<string> getIdentifierWildCardRelation(const string &label) override;

    /**
     * @return A vector of stmt pairs that fulfils the synonym wildcard relation of while pattern.
     */
    vector<pair<string, string>> getSynonymWildCardRelation() override;
};