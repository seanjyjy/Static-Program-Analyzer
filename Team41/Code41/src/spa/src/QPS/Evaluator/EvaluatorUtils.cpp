#include "EvaluatorUtils.h"

// ============================================= GENERAL UTILS ======================================================

bool EvaluatorUtils::isStmtType(QueryDeclaration::design_entity_type type) {
    bool isTypeProcedure = isProcedure(type);
    bool isTypeConstant = isConstant(type);
    bool isTypeVariable = isVariable(type);

    if (isTypeProcedure || isTypeVariable || isTypeConstant) {
        return false;
    }

    return true;
}

bool EvaluatorUtils::isModifiesStmtType(QueryDeclaration::design_entity_type type) {
    return isStmtType(type) && !isPrint(type);
}

bool EvaluatorUtils::isUsesStmtType(QueryDeclaration::design_entity_type type) {
    return isStmtType(type) && !isRead(type);
}

bool EvaluatorUtils::isVariable(QueryDeclaration::design_entity_type type) {
    return type == QueryDeclaration::design_entity_type::VARIABLE;
}

bool EvaluatorUtils::isProcedure(QueryDeclaration::design_entity_type type) {
    return type == QueryDeclaration::design_entity_type::PROCEDURE;
}

bool EvaluatorUtils::isConstant(QueryDeclaration::design_entity_type type) {
    return type == QueryDeclaration::design_entity_type::CONSTANT;
}

bool EvaluatorUtils::isPrint(QueryDeclaration::design_entity_type type) {
    return type == QueryDeclaration::design_entity_type::PRINT;
}

bool EvaluatorUtils::isRead(QueryDeclaration::design_entity_type type) {
    return type == QueryDeclaration::design_entity_type::READ;
}

bool EvaluatorUtils::isCall(QueryDeclaration::design_entity_type type) {
    return type == QueryDeclaration::design_entity_type::CALL;
}

bool EvaluatorUtils::isIntegerSynonym(ClauseVariable* left, ClauseVariable* right) {
    return left->isInteger() && right->isSynonym();
}

bool EvaluatorUtils::isIdentifierSynonym(ClauseVariable* left, ClauseVariable* right) {
    return left->isIdentifier() && right->isSynonym();
}

bool EvaluatorUtils::isWildCardSynonym(ClauseVariable* left, ClauseVariable* right) {
    return left->isWildCard() && right->isSynonym();
}

bool EvaluatorUtils::isSynonymSynonym(ClauseVariable* left, ClauseVariable* right) {
    return left->isSynonym() && right->isSynonym();
}

bool EvaluatorUtils::isSynonymInteger(ClauseVariable* left, ClauseVariable* right) {
    return left->isSynonym() && right->isInteger();
}

bool EvaluatorUtils::isSynonymIdentifier(ClauseVariable* left, ClauseVariable* right) {
    return left->isSynonym() && right->isIdentifier();
}

bool EvaluatorUtils::isSynonymWildCard(ClauseVariable* left, ClauseVariable* right) {
    return left->isSynonym() && right->isWildCard();
}

// ============================================= STMT UTILS ======================================================

bool EvaluatorUtils::StmtUtils::isIntegerInteger(ClauseVariable* left, ClauseVariable* right) {
    return left->isInteger() && right->isInteger();
}

bool EvaluatorUtils::StmtUtils::isValidIntegerSynonym(ClauseVariable* left, ClauseVariable* right) {
    return isIntegerSynonym(left, right) && isStmtType(right->getDesignEntityType());
}

bool EvaluatorUtils::StmtUtils::isIntegerWildCard(ClauseVariable* left, ClauseVariable* right) {
    return left->isInteger() && right->isWildCard();
}

bool EvaluatorUtils::StmtUtils::isValidSynonymInteger(ClauseVariable* left, ClauseVariable* right) {
    return isSynonymInteger(left, right) && isStmtType(left->getDesignEntityType());
}

bool EvaluatorUtils::StmtUtils::isValidSynonymSynonym(ClauseVariable* left, ClauseVariable* right) {
    return isSynonymSynonym(left, right)
           && isStmtType(left->getDesignEntityType())
           && isStmtType(right->getDesignEntityType());
}

bool EvaluatorUtils::StmtUtils::isValidSynonymWildCard(ClauseVariable* left, ClauseVariable* right) {
    return isSynonymWildCard(left, right) && isStmtType(left->getDesignEntityType());
}

bool EvaluatorUtils::StmtUtils::isWildCardInteger(ClauseVariable* left, ClauseVariable* right) {
    return left->isWildCard() && right->isInteger();
}

bool EvaluatorUtils::StmtUtils::isValidWildCardSynonym(ClauseVariable* left, ClauseVariable* right) {
    return isWildCardSynonym(left, right) && isStmtType(right->getDesignEntityType());
}

bool EvaluatorUtils::StmtUtils::isWildCardWildCard(ClauseVariable* left, ClauseVariable* right) {
    return left->isWildCard() && right->isWildCard();
}

// ============================================= PROC UTILS ======================================================

bool EvaluatorUtils::ProcUtils::isIdentifierIdentifier(ClauseVariable *left, ClauseVariable *right) {
    return left->isIdentifier() && right->isIdentifier();
}

bool EvaluatorUtils::ProcUtils::isValidIdentifierSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isIdentifierSynonym(left, right) && isVariable(right->getDesignEntityType());
}

bool EvaluatorUtils::ProcUtils::isIdentifierWildCard(ClauseVariable *left, ClauseVariable *right) {
    return left->isIdentifier() && right->isWildCard();
}

bool EvaluatorUtils::ProcUtils::isValidSynonymIdentifier(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymIdentifier(left, right) && isProcedure(left->getDesignEntityType());
}

bool EvaluatorUtils::ProcUtils::isValidSynonymSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymSynonym(left, right)
        && isProcedure(left->getDesignEntityType())
        && isVariable(right->getDesignEntityType());
}

bool EvaluatorUtils::ProcUtils::isValidSynonymWildCard(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymWildCard(left, right) && isProcedure(left->getDesignEntityType());
}

// ============================================= S UTILS ======================================================

bool EvaluatorUtils::SUtils::isIntegerIdentifier(ClauseVariable* left, ClauseVariable* right) {
    return left->isInteger() && right->isIdentifier();
}

bool EvaluatorUtils::SUtils::isValidIntegerSynonym(ClauseVariable* left, ClauseVariable* right) {
    return isIntegerSynonym(left, right) && isVariable(right->getDesignEntityType());
}

bool EvaluatorUtils::SUtils::isIntegerWildCard(ClauseVariable* left, ClauseVariable* right) {
    return left->isInteger() && right->isWildCard();
}

bool EvaluatorUtils::SUtils::isValidModifiesSynonymIdentifier(ClauseVariable* left, ClauseVariable* right) {
    return isSynonymIdentifier(left, right) && isModifiesStmtType(left->getDesignEntityType());
}

bool EvaluatorUtils::SUtils::isValidUsesSynonymIdentifier(ClauseVariable* left, ClauseVariable* right) {
    return isSynonymIdentifier(left, right) && isUsesStmtType(left->getDesignEntityType());
}

bool EvaluatorUtils::SUtils::isValidModifiesSynonymSynonym(ClauseVariable* left, ClauseVariable* right) {
    return isSynonymSynonym(left, right)
        && isModifiesStmtType(left->getDesignEntityType())
        && isVariable(right->getDesignEntityType());
}

bool EvaluatorUtils::SUtils::isValidUsesSynonymSynonym(ClauseVariable* left, ClauseVariable* right) {
    return isSynonymSynonym(left, right)
           && isUsesStmtType(left->getDesignEntityType())
           && isVariable(right->getDesignEntityType());
}


bool EvaluatorUtils::SUtils::isValidModifiesSynonymWildCard(ClauseVariable* left, ClauseVariable* right) {
    return isSynonymWildCard(left, right) && isModifiesStmtType(left->getDesignEntityType());
}

bool EvaluatorUtils::SUtils::isValidUsesSynonymWildCard(ClauseVariable* left, ClauseVariable* right) {
    return isSynonymWildCard(left, right) && isUsesStmtType(left->getDesignEntityType());
}
