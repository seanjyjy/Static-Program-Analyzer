#pragma once

#include "IfWhilePatternEvaluator.h"
#include "Common/AstNode/TNode.h"
#include "QPS/QO/PatternClause.h"

class IfPatternEvaluator : public IfWhilePatternEvaluator {
public:
    explicit IfPatternEvaluator(PKBClient *pkb);

    /**
     * @return A set of stmts that fulfils the wildcard wildcard relation of if pattern.
     */
    unordered_set<string> getWildCardWildCardRelation() override;

    /**
     * @return A set of stmts that fulfils the identifier wildcard relation of if pattern.
     */
    unordered_set<string> getIdentifierWildCardRelation(const string &label) override;

    /**
     * @return A vector of stmt pairs that fulfils the synonym wildcard relation of if pattern.
     */
    vector<pair<string, string>> getSynonymWildCardRelation() override;
};