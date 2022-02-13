#include "EvaluatorUtils.h"

bool EvaluatorUtils::isStmtType(QueryDeclaration::design_entity_type type) {
    bool isProcedure = type == QueryDeclaration::design_entity_type::PROCEDURE;
    bool isConstant = type == QueryDeclaration::design_entity_type::CONSTANT;
    bool isVariable = EvaluatorUtils::isVariable(type);

    if (isProcedure || isVariable || isConstant) {
        return false;
    }

    return true;
}

bool EvaluatorUtils::isVariable(QueryDeclaration::design_entity_type type) {
    return type == QueryDeclaration::design_entity_type::VARIABLE;
}

bool EvaluatorUtils::isProcedure(QueryDeclaration::design_entity_type type) {
    return type == QueryDeclaration::design_entity_type::PROCEDURE;
}

// ============================================= STMT UTILS ======================================================

bool EvaluatorUtils::StmtUtils::isIntegerInteger(ClauseVariable* left, ClauseVariable* right) {
    return left->isInteger() && right->isInteger();
}

bool EvaluatorUtils::StmtUtils::isValidIntegerSynonym(ClauseVariable* left, ClauseVariable* right) {
    return left->isInteger() && right->isSynonym() && EvaluatorUtils::isStmtType(right->getDesignEntityType());
}

bool EvaluatorUtils::StmtUtils::isIntegerWildCard(ClauseVariable* left, ClauseVariable* right) {
    return left->isInteger() && right->isWildCard();
}

bool EvaluatorUtils::StmtUtils::isValidSynonymInteger(ClauseVariable* left, ClauseVariable* right) {
    return left->isSynonym() && right->isInteger() && EvaluatorUtils::isStmtType(left->getDesignEntityType());
}

bool EvaluatorUtils::StmtUtils::isValidSynonymSynonym(ClauseVariable* left, ClauseVariable* right) {
    return left->isSynonym() &&
           right->isSynonym() &&
           EvaluatorUtils::isStmtType(left->getDesignEntityType()) &&
           EvaluatorUtils::isStmtType(right->getDesignEntityType());
}

bool EvaluatorUtils::StmtUtils::isValidSynonymWildCard(ClauseVariable* left, ClauseVariable* right) {
    return left->isSynonym() && right->isWildCard() && EvaluatorUtils::isStmtType(left->getDesignEntityType());
}

bool EvaluatorUtils::StmtUtils::isWildCardInteger(ClauseVariable* left, ClauseVariable* right) {
    return left->isWildCard() && right->isInteger();
}

bool EvaluatorUtils::StmtUtils::isValidWildCardSynonym(ClauseVariable* left, ClauseVariable* right) {
    return left->isWildCard() && right->isSynonym() && EvaluatorUtils::isStmtType(right->getDesignEntityType());
}

bool EvaluatorUtils::StmtUtils::isWildCardWildCard(ClauseVariable* left, ClauseVariable* right) {
    return left->isWildCard() && right->isWildCard();
}

// ============================================= PROC UTILS ======================================================

bool EvaluatorUtils::ProcUtils::isIdentifierIdentifier(ClauseVariable *left, ClauseVariable *right) {
    return left->isIdentifier() && right->isIdentifier();
}

bool EvaluatorUtils::ProcUtils::isValidIdentifierSynonym(ClauseVariable *left, ClauseVariable *right) {
    return left->isIdentifier() && right->isSynonym() && EvaluatorUtils::isVariable(right->getDesignEntityType());
}

bool EvaluatorUtils::ProcUtils::isIdentifierWildCard(ClauseVariable *left, ClauseVariable *right) {
    return left->isIdentifier() && right->isWildCard();
}

bool EvaluatorUtils::ProcUtils::isValidSynonymIdentifier(ClauseVariable *left, ClauseVariable *right) {
    return left->isSynonym() && right->isIdentifier() && EvaluatorUtils::isProcedure(left->getDesignEntityType());
}

bool EvaluatorUtils::ProcUtils::isValidSynonymSynonym(ClauseVariable *left, ClauseVariable *right) {
    return left->isSynonym() &&
        right->isSynonym() &&
        EvaluatorUtils::isProcedure(left->getDesignEntityType()) &&
        EvaluatorUtils::isVariable(right->getDesignEntityType());
}

bool EvaluatorUtils::ProcUtils::isValidSynonymWildCard(ClauseVariable *left, ClauseVariable *right) {
    return left->isSynonym() && right->isWildCard() && EvaluatorUtils::isProcedure(left->getDesignEntityType());
}

// ============================================= S UTILS ======================================================

bool EvaluatorUtils::SUtils::isIntegerIdentifier(ClauseVariable* left, ClauseVariable* right) {
    return left->isInteger() && right->isIdentifier();
}

bool EvaluatorUtils::SUtils::isValidIntegerSynonym(ClauseVariable* left, ClauseVariable* right) {
    return left->isInteger() && right->isSynonym() && EvaluatorUtils::isVariable(right->getDesignEntityType());
}

bool EvaluatorUtils::SUtils::isIntegerWildCard(ClauseVariable* left, ClauseVariable* right) {
    return left->isInteger() && right->isWildCard();
}

bool EvaluatorUtils::SUtils::isValidSynonymIdentifier(ClauseVariable* left, ClauseVariable* right) {
    return left->isSynonym() && right->isIdentifier() && EvaluatorUtils::isStmtType(left->getDesignEntityType());
}

bool EvaluatorUtils::SUtils::isValidSynonymSynonym(ClauseVariable* left, ClauseVariable* right) {
    return left->isSynonym()
        && right->isSynonym()
        && EvaluatorUtils::isStmtType(left->getDesignEntityType())
        && EvaluatorUtils::isVariable(right->getDesignEntityType());
}

bool EvaluatorUtils::SUtils::isValidSynonymWildCard(ClauseVariable* left, ClauseVariable* right) {
    return left->isSynonym() && right->isWildCard() && EvaluatorUtils::isStmtType(left->getDesignEntityType());
}
