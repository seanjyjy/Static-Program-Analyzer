#include "AffectEvaluator.h"

AffectEvaluator::AffectEvaluator(PKBClient *pkb) : GenericClauseEvaluator(pkb) {
    this->assignStmts = pkb->getAssigns();
}

Table *AffectEvaluator::evaluateClause(ClauseVariable left, ClauseVariable right) {
    if (EvaluatorUtils::StmtUtils::isIntegerInteger(&left, &right) &&
        isValidAssignAssign(left.getLabel(), right.getLabel())) {
        return evaluateIntegerInteger(left, right);
    }
    
    if (EvaluatorUtils::StmtUtils::isValidIntegerSynonym(&left, &right)
        && isValidAssignStmt(left.getLabel()) && isValidAssignSyn(right)) {
        return evaluateIntegerSynonym(left, right);
    }

    if (EvaluatorUtils::StmtUtils::isIntegerWildCard(&left, &right) && isValidAssignStmt(left.getLabel())) {
        return evaluateIntegerWildCard(left);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymInteger(&left, &right)
        && isValidAssignStmt(right.getLabel()) && isValidAssignSyn(left)) {
        return evaluateSynonymInteger(left, right);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymSynonym(&left, &right) && isValidAssignSynSyn(left, right)) {
        return evaluateSynonymSynonym(left, right);
    }

    if (EvaluatorUtils::StmtUtils::isValidSynonymWildCard(&left, &right) && isValidAssignSyn(left)) {
        return evaluateSynonymWildCard(left);
    }

    if (EvaluatorUtils::StmtUtils::isWildCardInteger(&left, &right) && isValidAssignStmt(right.getLabel())) {
        return evaluateWildCardInteger(right);
    }

    if (EvaluatorUtils::StmtUtils::isValidWildCardSynonym(&left, &right) && isValidAssignSyn(right)) {
        return evaluateWildCardSynonym(right);
    }

    if (EvaluatorUtils::isWildCardWildCard(&left, &right)) {
        return evaluateWildCardWildCard();
    }

    throw SemanticException("Invalid query provided for Affect type");
}

bool AffectEvaluator::isValidAssignAssign(const string &stmt1, const string &stmt2) {
    // checks if both statements are an assign statement
    if (assignStmts.find(stmt1) == assignStmts.end() || assignStmts.find(stmt2) == assignStmts.end()) {
        return false;
    }

    // checks if both statements are in the same procedure
    CFGNode* root = pkb->getRootCFG();
    vector<CFGNode *> children = root->getChildren();
    int stmt1Val = stoi(stmt1);
    int stmt2Val = stoi(stmt2);
    for (auto& child : root->getChildren()) {
        int procStart = stoi(child->getStmtNum());
        if (stmt1Val < procStart && stmt2Val >= procStart) {
            return false;
        }
    }
    
    return true;
}

bool AffectEvaluator::isValidAssignStmt(const string &stmt) {
    if (assignStmts.find(stmt) == assignStmts.end()) {
        return false;
    }

    return true;
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

Table *AffectEvaluator::evaluateIntegerInteger(const ClauseVariable& left, const ClauseVariable& right) {
    bool affectResult = getIntegerIntegerRelation(left.getLabel(), right.getLabel());
    return buildBooleanTable(affectResult);
}

Table *AffectEvaluator::evaluateIntegerSynonym(const ClauseVariable& left, ClauseVariable right) {
    unordered_set<string> affecteds = getIntegerSynonymRelation(left.getLabel());
    return buildSingleSynonymTable(affecteds, right);
}

Table *AffectEvaluator::evaluateIntegerWildCard(const ClauseVariable& left) {
    unordered_set<string> affecteds = getIntegerWildCardRelation(left.getLabel());
    return buildBooleanTable(!affecteds.empty());
}

Table *AffectEvaluator::evaluateSynonymInteger(ClauseVariable left, const ClauseVariable& right) {
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

Table *AffectEvaluator::evaluateWildCardInteger(const ClauseVariable& right) {
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
