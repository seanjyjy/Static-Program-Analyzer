#include "AffectEvaluator.h"

AffectEvaluator::AffectEvaluator(PKBClient *pkb, AffectsKBProxy *proxy) :
        GenericClauseEvaluator(pkb), affectsKBProxy(proxy) {}

Table *AffectEvaluator::evaluateClause(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    if (EvaluatorUtils::StmtUtils::isIntegerInteger(&left, &right)) {
        if (!EvaluatorUtils::isWithinLimit(left, right, pkb) || !isValidAssignAssign(left, right))
            return new FalseTable();

        return evaluateIntegerInteger(left, right);
    }

    if (EvaluatorUtils::StmtUtils::isValidIntegerSynonym(&left, &right)) {
        if (!EvaluatorUtils::isWithinLimit(left, pkb) || !isValidAssignStmt(left) || !isValidAssignSyn(right))
            return new FalseTable();

        return evaluateIntegerSynonym(left, right, canSimplify);
    }

    if (EvaluatorUtils::StmtUtils::isIntegerWildCard(&left, &right)) {
        if (!EvaluatorUtils::isWithinLimit(left, pkb) || !isValidAssignStmt(left))
            return new FalseTable();

        return evaluateIntegerWildCard(left);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymInteger(&left, &right)) {
        if (!EvaluatorUtils::isWithinLimit(right, pkb) || !isValidAssignStmt(right) || !isValidAssignSyn(left))
            return new FalseTable();

        return evaluateSynonymInteger(left, right, canSimplify);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymSynonym(&left, &right)) {
        if (!isValidAssignSynSyn(left, right))
            return new FalseTable();

        return evaluateSynonymSynonym(left, right, canSimplify);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymWildCard(&left, &right)) {
        if (!isValidAssignSyn(left))
            return new FalseTable();

        return evaluateSynonymWildCard(left, canSimplify);
    }

    if (EvaluatorUtils::StmtUtils::isWildCardInteger(&left, &right)) {
        if (!EvaluatorUtils::isWithinLimit(right, pkb) || !isValidAssignStmt(right))
            return new FalseTable();

        return evaluateWildCardInteger(right);
    }

    if (EvaluatorUtils::StmtUtils::isValidWildCardSynonym(&left, &right)) {
        if (!isValidAssignSyn(right))
            return new FalseTable();

        return evaluateWildCardSynonym(right, canSimplify);
    }

    if (EvaluatorUtils::isWildCardWildCard(&left, &right)) {
        return evaluateWildCardWildCard();
    }

    throw SemanticException("Invalid query provided for Affect type");
}

bool AffectEvaluator::isValidAssignAssign(ClauseVariable &left, ClauseVariable &right) {
    // checks if both statements are an assign statement
    return pkb->isAssignStmt(left.getLabel()) && pkb->isAssignStmt(right.getLabel());
}

bool AffectEvaluator::isValidAssignStmt(ClauseVariable &variable) {
    return pkb->isAssignStmt(variable.getLabel());
}

bool AffectEvaluator::isValidAssignSyn(ClauseVariable &left) {
    auto type = left.getDesignEntityType();

    return type->isAssign() || type->isStmt();
}

bool AffectEvaluator::isValidAssignSynSyn(ClauseVariable &left, ClauseVariable &right) {
    auto leftType = left.getDesignEntityType();
    auto rightType = right.getDesignEntityType();

    return (leftType->isAssign() || leftType->isStmt()) && (rightType->isAssign() || rightType->isStmt());
}

Table *AffectEvaluator::evaluateIntegerInteger(const ClauseVariable &left, const ClauseVariable &right) {
    bool affectResult = getIntegerIntegerRelation(left.getLabel(), right.getLabel());
    return buildBooleanTable(affectResult);
}

Table *AffectEvaluator::evaluateIntegerSynonym(const ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    unordered_set<string> affecteds = getIntegerSynonymRelation(left.getLabel());
    return buildSingleSynonymTable(affecteds, right, canSimplify);
}

Table *AffectEvaluator::evaluateIntegerWildCard(const ClauseVariable &left) {
    bool hasAffecteds = getIntegerWildCardRelation(left.getLabel());
    return buildBooleanTable(hasAffecteds);
}

Table *AffectEvaluator::evaluateSynonymInteger(ClauseVariable &left, const ClauseVariable &right, bool canSimplify) {
    unordered_set<string> affectings = getSynonymIntegerRelation(right.getLabel());
    return buildSingleSynonymTable(affectings, left, canSimplify);
}

Table *AffectEvaluator::evaluateSynonymSynonym(ClauseVariable &left, ClauseVariable &right, bool canSimplify) {
    vector<pair<string, string>> affectingAffectedPairs = getSynonymSynonymRelation();
    return buildSynonymSynonymTable(affectingAffectedPairs, left, right, canSimplify);
}

Table *AffectEvaluator::evaluateSynonymWildCard(ClauseVariable &left, bool canSimplify) {
    unordered_set<string> affectings = getSynonymWildCardRelation();
    return buildSingleSynonymTable(affectings, left, canSimplify);
}

Table *AffectEvaluator::evaluateWildCardInteger(const ClauseVariable &right) {
    bool hasAffectings = getWildCardIntegerRelation(right.getLabel());
    return buildBooleanTable(hasAffectings);
}

Table *AffectEvaluator::evaluateWildCardSynonym(ClauseVariable &right, bool canSimplify) {
    unordered_set<string> affecteds = getWildCardSynonymRelation();
    return buildSingleSynonymTable(affecteds, right, canSimplify);
}

Table *AffectEvaluator::evaluateWildCardWildCard() {
    bool hasAffectingAffectedPairs = getWildCardWildCardRelation();
    return buildBooleanTable(hasAffectingAffectedPairs);
}

bool AffectEvaluator::getIntegerWildCardRelation(const string &label) {
    return affectsKBProxy->isAffectingSomeStmt(label);
}

bool AffectEvaluator::getWildCardIntegerRelation(const string &label) {
    return affectsKBProxy->isAffectedBySomeStmt(label);
}

bool AffectEvaluator::getWildCardWildCardRelation() {
    return affectsKBProxy->hasSomeAffectsAll();
}