#include "AffectEvaluator.h"

AffectEvaluator::AffectEvaluator(PKBClient *pkb) : GenericClauseEvaluator(pkb) {}

Table *AffectEvaluator::evaluateClause(ClauseVariable left, ClauseVariable right) {
    if (EvaluatorUtils::StmtUtils::isIntegerInteger(&left, &right)) {
        if (!EvaluatorUtils::isWithinLimit(left, right, pkb) || !isValidAssignAssign(left, right))
            return new FalseTable();

        return evaluateIntegerInteger(left, right);
    }

    if (EvaluatorUtils::StmtUtils::isValidIntegerSynonym(&left, &right)) {
        if (!EvaluatorUtils::isWithinLimit(left, pkb) || !isValidAssignStmt(left) || !isValidAssignSyn(right))
            return new FalseTable();

        return evaluateIntegerSynonym(left, right);
    }

    if (EvaluatorUtils::StmtUtils::isIntegerWildCard(&left, &right)) {
        if (!EvaluatorUtils::isWithinLimit(left, pkb) || !isValidAssignStmt(left))
            return new FalseTable();

        return evaluateIntegerWildCard(left);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymInteger(&left, &right)) {
        if (!EvaluatorUtils::isWithinLimit(right, pkb) || !isValidAssignStmt(right) || !isValidAssignSyn(left))
            return new FalseTable();

        return evaluateSynonymInteger(left, right);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymSynonym(&left, &right)) {
        if (!isValidAssignSynSyn(left, right))
            return new FalseTable();

        return evaluateSynonymSynonym(left, right);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymWildCard(&left, &right)) {
        if (!isValidAssignSyn(left))
            return new FalseTable();

        return evaluateSynonymWildCard(left);
    }

    if (EvaluatorUtils::StmtUtils::isWildCardInteger(&left, &right)) {
        if (!EvaluatorUtils::isWithinLimit(right, pkb) || !isValidAssignStmt(right))
            return new FalseTable();

        return evaluateWildCardInteger(right);
    }

    if (EvaluatorUtils::StmtUtils::isValidWildCardSynonym(&left, &right)) {
        if (!isValidAssignSyn(right))
            return new FalseTable();

        return evaluateWildCardSynonym(right);
    }

    if (EvaluatorUtils::isWildCardWildCard(&left, &right)) {
        return evaluateWildCardWildCard();
    }

    throw SemanticException("Invalid query provided for Affect type");
}

bool AffectEvaluator::isValidAssignAssign(ClauseVariable& left, ClauseVariable& right) {
    // checks if both statements are an assign statement
    return pkb->isAssignStmt(left.getLabel()) && pkb->isAssignStmt(right.getLabel());
}

bool AffectEvaluator::isValidAssignStmt(ClauseVariable& variable) {
    return pkb->isAssignStmt(variable.getLabel());
}

bool AffectEvaluator::isValidAssignSyn(ClauseVariable &left) {
    auto type = left.getDesignEntityType();

    return EvaluatorUtils::isAssign(type) || EvaluatorUtils::isStmt(type);
}

bool AffectEvaluator::isValidAssignSynSyn(ClauseVariable &left, ClauseVariable &right) {
    auto leftType = left.getDesignEntityType();
    auto rightType = right.getDesignEntityType();

    return (EvaluatorUtils::isAssign(leftType) || EvaluatorUtils::isStmt(leftType))
           && (EvaluatorUtils::isAssign(rightType) || EvaluatorUtils::isStmt(rightType));
}

Table *AffectEvaluator::evaluateIntegerInteger(const ClauseVariable &left, const ClauseVariable &right) {
    bool affectResult = getIntegerIntegerRelation(left.getLabel(), right.getLabel());
    return buildBooleanTable(affectResult);
}

Table *AffectEvaluator::evaluateIntegerSynonym(const ClauseVariable &left, ClauseVariable right) {
    unordered_set<string> affecteds = getIntegerSynonymRelation(left.getLabel());
    return buildSingleSynonymTable(affecteds, right);
}

Table *AffectEvaluator::evaluateIntegerWildCard(const ClauseVariable &left) {
    unordered_set<string> affecteds = getIntegerWildCardRelation(left.getLabel());
    return buildBooleanTable(!affecteds.empty());
}

Table *AffectEvaluator::evaluateSynonymInteger(ClauseVariable left, const ClauseVariable &right) {
    unordered_set<string> affectings = getSynonymIntegerRelation(right.getLabel());
    return buildSingleSynonymTable(affectings, left);
}

Table *AffectEvaluator::evaluateSynonymSynonym(ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> affectingAffectedPairs = getSynonymSynonymRelation();
    return buildSynonymSynonymTable(affectingAffectedPairs, left, right);
}

Table *AffectEvaluator::evaluateSynonymWildCard(ClauseVariable left) {
    unordered_set<string> affectings = getSynonymWildCardRelation();
    return buildSingleSynonymTable(affectings, left);
}

Table *AffectEvaluator::evaluateWildCardInteger(const ClauseVariable &right) {
    unordered_set<string> affectings = getWildCardIntegerRelation(right.getLabel());
    return buildBooleanTable(!affectings.empty());
}

Table *AffectEvaluator::evaluateWildCardSynonym(ClauseVariable right) {
    unordered_set<string> affecteds = getWildCardSynonymRelation();
    return buildSingleSynonymTable(affecteds, right);
}

Table *AffectEvaluator::evaluateWildCardWildCard() {
    vector<pair<string, string>> affectingAffectedPairs = getWildCardWildCardRelation();
    return buildBooleanTable(!affectingAffectedPairs.empty());
}
