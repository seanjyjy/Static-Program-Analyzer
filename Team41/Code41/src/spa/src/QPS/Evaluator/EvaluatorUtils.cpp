#include "EvaluatorUtils.h"

// ============================================= GENERAL UTILS ======================================================

bool EvaluatorUtils::isStmtType(Entities *type) {
    return !(type->isProcedure() || type->isConstant() || type->isVariable());
}

bool EvaluatorUtils::isModifiesStmtType(Entities *type) {
    return isStmtType(type) && !type->isPrint();
}

bool EvaluatorUtils::isUsesStmtType(Entities *type) {
    return isStmtType(type) && !type->isRead();
}

bool EvaluatorUtils::isVariableSynonym(ClauseVariable *var) {
    return var->isSynonym() && var->getDesignEntityType()->isVariable();
}

bool EvaluatorUtils::isIntegerSynonym(ClauseVariable *left, ClauseVariable *right) {
    return left->isInteger() && right->isSynonym();
}

bool EvaluatorUtils::isIdentifierSynonym(ClauseVariable *left, ClauseVariable *right) {
    return left->isIdentifier() && right->isSynonym();
}

bool EvaluatorUtils::isWildCardSynonym(ClauseVariable *left, ClauseVariable *right) {
    return left->isWildCard() && right->isSynonym();
}

bool EvaluatorUtils::isSynonymSynonym(ClauseVariable *left, ClauseVariable *right) {
    return left->isSynonym() && right->isSynonym();
}

bool EvaluatorUtils::isSynonymInteger(ClauseVariable *left, ClauseVariable *right) {
    return left->isSynonym() && right->isInteger();
}

bool EvaluatorUtils::isSynonymIdentifier(ClauseVariable *left, ClauseVariable *right) {
    return left->isSynonym() && right->isIdentifier();
}

bool EvaluatorUtils::isSynonymWildCard(ClauseVariable *left, ClauseVariable *right) {
    return left->isSynonym() && right->isWildCard();
}

bool EvaluatorUtils::isWildCardIdentifier(ClauseVariable *left, ClauseVariable *right) {
    return left->isWildCard() && right->isIdentifier();
}

bool EvaluatorUtils::isWildCardWildCard(ClauseVariable *left, ClauseVariable *right) {
    return left->isWildCard() && right->isWildCard();
}

bool EvaluatorUtils::validateDeclarations(vector<QueryDeclaration> &declarations) {
    unordered_set<string> set;
    for (QueryDeclaration &declaration: declarations) {
        if (set.find(declaration.getSynonym()) != set.end()) {
            return false;
        }
        set.insert(declaration.getSynonym());
    }
    return true;
}

bool EvaluatorUtils::isWithinLimit(ClauseVariable &variable, PKBClient *pkb) {
    return pkb->hasStmt(variable.getLabel());
}

bool EvaluatorUtils::isWithinLimit(ClauseVariable &left, ClauseVariable &right, PKBClient *pkb) {
    return isWithinLimit(left, pkb) && isWithinLimit(right, pkb);
}

// ============================================= STMT UTILS ======================================================

bool EvaluatorUtils::StmtUtils::isIntegerInteger(ClauseVariable *left, ClauseVariable *right) {
    return left->isInteger() && right->isInteger();
}

bool EvaluatorUtils::StmtUtils::isValidIntegerSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isIntegerSynonym(left, right) && isStmtType(right->getDesignEntityType());
}

bool EvaluatorUtils::StmtUtils::isIntegerWildCard(ClauseVariable *left, ClauseVariable *right) {
    return left->isInteger() && right->isWildCard();
}

bool EvaluatorUtils::StmtUtils::isValidSynonymInteger(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymInteger(left, right) && isStmtType(left->getDesignEntityType());
}

bool EvaluatorUtils::StmtUtils::isValidSynonymSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymSynonym(left, right)
           && isStmtType(left->getDesignEntityType())
           && isStmtType(right->getDesignEntityType());
}

bool EvaluatorUtils::StmtUtils::isValidSynonymWildCard(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymWildCard(left, right) && isStmtType(left->getDesignEntityType());
}

bool EvaluatorUtils::StmtUtils::isWildCardInteger(ClauseVariable *left, ClauseVariable *right) {
    return left->isWildCard() && right->isInteger();
}

bool EvaluatorUtils::StmtUtils::isValidWildCardSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isWildCardSynonym(left, right) && isStmtType(right->getDesignEntityType());
}

// ============================================= PROC UTILS ======================================================

bool EvaluatorUtils::ProcUtils::isIdentifierIdentifier(ClauseVariable *left, ClauseVariable *right) {
    return left->isIdentifier() && right->isIdentifier();
}

bool EvaluatorUtils::ProcUtils::isValidIdentifierSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isIdentifierSynonym(left, right) && right->getDesignEntityType()->isVariable();
}

bool EvaluatorUtils::ProcUtils::isIdentifierWildCard(ClauseVariable *left, ClauseVariable *right) {
    return left->isIdentifier() && right->isWildCard();
}

bool EvaluatorUtils::ProcUtils::isValidSynonymIdentifier(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymIdentifier(left, right) && left->getDesignEntityType()->isProcedure();
}

bool EvaluatorUtils::ProcUtils::isValidSynonymSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymSynonym(left, right)
           && left->getDesignEntityType()->isProcedure()
           && right->getDesignEntityType()->isVariable();
}

bool EvaluatorUtils::ProcUtils::isValidSynonymWildCard(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymWildCard(left, right) && left->getDesignEntityType()->isProcedure();
}

// ============================================= S UTILS ======================================================

bool EvaluatorUtils::SUtils::isIntegerIdentifier(ClauseVariable *left, ClauseVariable *right) {
    return left->isInteger() && right->isIdentifier();
}

bool EvaluatorUtils::SUtils::isValidIntegerSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isIntegerSynonym(left, right) && right->getDesignEntityType()->isVariable();
}

bool EvaluatorUtils::SUtils::isIntegerWildCard(ClauseVariable *left, ClauseVariable *right) {
    return left->isInteger() && right->isWildCard();
}

bool EvaluatorUtils::SUtils::isValidModifiesSynonymIdentifier(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymIdentifier(left, right) && isModifiesStmtType(left->getDesignEntityType());
}

bool EvaluatorUtils::SUtils::isValidUsesSynonymIdentifier(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymIdentifier(left, right) && isUsesStmtType(left->getDesignEntityType());
}

bool EvaluatorUtils::SUtils::isValidModifiesSynonymSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymSynonym(left, right)
           && isModifiesStmtType(left->getDesignEntityType())
           && right->getDesignEntityType()->isVariable();
}

bool EvaluatorUtils::SUtils::isValidUsesSynonymSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymSynonym(left, right)
           && isUsesStmtType(left->getDesignEntityType())
           && right->getDesignEntityType()->isVariable();
}


bool EvaluatorUtils::SUtils::isValidModifiesSynonymWildCard(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymWildCard(left, right) && isModifiesStmtType(left->getDesignEntityType());
}

bool EvaluatorUtils::SUtils::isValidUsesSynonymWildCard(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymWildCard(left, right) && isUsesStmtType(left->getDesignEntityType());
}

// ============================================ Calls Utils ====================================================

bool EvaluatorUtils::CallsUtils::isValidCallsIdentifierSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isIdentifierSynonym(left, right) && right->getDesignEntityType()->isProcedure();
}

bool EvaluatorUtils::CallsUtils::isValidCallsSynonymIdentifier(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymIdentifier(left, right) && left->getDesignEntityType()->isProcedure();
}

bool EvaluatorUtils::CallsUtils::isValidCallsSynonymSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymSynonym(left, right)
           && left->getDesignEntityType()->isProcedure()
           && right->getDesignEntityType()->isProcedure();
}

bool EvaluatorUtils::CallsUtils::isValidCallsSynonymWildCard(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymWildCard(left, right) && left->getDesignEntityType()->isProcedure();
}

bool EvaluatorUtils::CallsUtils::isValidCallsWildCardSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isWildCardSynonym(left, right) && right->getDesignEntityType()->isProcedure();
}

// ============================================ Attr Utils ====================================================

bool EvaluatorUtils::AttrUtils::isValidSelectable(Selectable *target) {
    return target->getType() != Selectable::ATTR_REF || isProcProcNameAttr(target) || isCallProcNameAttr(target) ||
           isReadVarNameAttr(target) || isPrintVarNameAttr(target) || isVarVarNameAttr(target) ||
           isConstantValueAttr(target) || isStmtStmtNumAttr(target);
}

bool EvaluatorUtils::AttrUtils::isProcProcNameAttr(Selectable *target) {
    return target->getAttr() == Selectable::PROC_NAME && target->getSynonym().getType()->isProcedure();
}

bool EvaluatorUtils::AttrUtils::isCallProcNameAttr(Selectable *target) {
    return target->getAttr() == Selectable::PROC_NAME && target->getSynonym().getType()->isCall();
}

bool EvaluatorUtils::AttrUtils::isReadVarNameAttr(Selectable *target) {
    return target->getAttr() == Selectable::VAR_NAME && target->getSynonym().getType()->isRead();
}

bool EvaluatorUtils::AttrUtils::isPrintVarNameAttr(Selectable *target) {
    return target->getAttr() == Selectable::VAR_NAME && target->getSynonym().getType()->isPrint();
}

bool EvaluatorUtils::AttrUtils::isVarVarNameAttr(Selectable *target) {
    return target->getAttr() == Selectable::VAR_NAME && target->getSynonym().getType()->isVariable();
}

bool EvaluatorUtils::AttrUtils::isConstantValueAttr(Selectable *target) {
    return target->getAttr() == Selectable::VALUE && target->getSynonym().getType()->isConstant();
}

bool EvaluatorUtils::AttrUtils::isStmtStmtNumAttr(Selectable *target) {
    return target->getAttr() == Selectable::STMT_NUM && isStmtType(target->getSynonym().getType());
}

bool EvaluatorUtils::AttrUtils::validateSelectTarget(SelectTarget *selection) {
    bool valid = selection->isBoolean();
    for (auto &target: selection->getSelectable()) {
        valid = valid || isValidSelectable(&target);
        if (!valid) return false;
    }
    return valid;
}

optional<string>
EvaluatorUtils::AttrUtils::getAttrFromSelectable(Selectable *target, const string &rawData, PKBClient *pkb) {
    QueryDeclaration declaration = target->getSynonym();

    if (isCallProcNameAttr(target)) {
        string procName = pkb->getCallsProcNameAttr(rawData);
        if (procName.empty()) return nullopt;
        return procName;

    } else if (isReadVarNameAttr(target)) {
        string varName = pkb->getReadVarNameAttr(rawData);
        if (varName.empty()) return nullopt;
        return varName;

    } else if (isPrintVarNameAttr(target)) {
        string varName = pkb->getPrintVarNameAttr(rawData);
        if (varName.empty()) return nullopt;
        return varName;

    } else if (isProcProcNameAttr(target) || isVarVarNameAttr(target) || isConstantValueAttr(target) ||
               isStmtStmtNumAttr(target)) {
        return rawData;

    } else {
        return nullopt;
    }
}

// ============================================ Pattern Utils ====================================================

bool EvaluatorUtils::PatternUtils::isWildCards(const vector<PatternVariable> &patternVariables,
                                               Entities *type) {

    size_t size = type->isIf() ? 2 : 1;

    if (patternVariables.size() < size) {
        return false;
    }

    return all_of(patternVariables.begin(), patternVariables.end(), [](PatternVariable patternVariable) {
        return patternVariable.isWildcard();
    });
}

bool EvaluatorUtils::PatternUtils::isWildCardWildCards(const ClauseVariable &variable,
                                                       const vector<PatternVariable> &patternVariables,
                                                       Entities *type) {

    return variable.isWildCard() && isWildCards(patternVariables, type);
}

bool EvaluatorUtils::PatternUtils::isIdentifierWildCards(const ClauseVariable &variable,
                                                         const vector<PatternVariable> &patternVariables,
                                                         Entities *type) {
    return variable.isIdentifier() && isWildCards(patternVariables, type);
}

bool EvaluatorUtils::PatternUtils::isValidSynonymWildCards(ClauseVariable &variable,
                                                           const vector<PatternVariable> &patternVariables,
                                                           Entities *type) {

    return isVariableSynonym(&variable) && isWildCards(patternVariables, type);
}