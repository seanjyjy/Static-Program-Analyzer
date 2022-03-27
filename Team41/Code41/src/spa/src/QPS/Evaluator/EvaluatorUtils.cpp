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

// Difference between isStmtType and isStmt is isStmtType checks for all different stmt type
bool EvaluatorUtils::isStmt(QueryDeclaration::design_entity_type type) {
    return type == QueryDeclaration::design_entity_type::STMT;
}

bool EvaluatorUtils::isWhile(QueryDeclaration::design_entity_type type) {
    return type == QueryDeclaration::design_entity_type::WHILE;
}

bool EvaluatorUtils::isIf(QueryDeclaration::design_entity_type type) {
    return type == QueryDeclaration::design_entity_type::IF;
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

bool EvaluatorUtils::isAssign(QueryDeclaration::design_entity_type type) {
    return type == QueryDeclaration::design_entity_type::ASSIGN;
}

bool EvaluatorUtils::isRead(QueryDeclaration::design_entity_type type) {
    return type == QueryDeclaration::design_entity_type::READ;
}

bool EvaluatorUtils::isCall(QueryDeclaration::design_entity_type type) {
    return type == QueryDeclaration::design_entity_type::CALL;
}

bool EvaluatorUtils::isVariableSynonym(ClauseVariable *var) {
    return var->isSynonym() && isVariable(var->getDesignEntityType());
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

bool EvaluatorUtils::validateDeclarations(vector<QueryDeclaration> declarations) {
    unordered_set<string> set;
    for (QueryDeclaration &declaration: declarations) {
        if (set.find(declaration.synonym) != set.end()) {
            return false;
        }
        set.insert(declaration.synonym);
    }
    return true;
}

bool EvaluatorUtils::isWithinLimit(ClauseVariable &variable, PKBClient* pkb) {
    return pkb->hasStmt(variable.getLabel());
}

bool EvaluatorUtils::isWithinLimit(ClauseVariable &left, ClauseVariable &right, PKBClient* pkb) {
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

bool EvaluatorUtils::SUtils::isIntegerIdentifier(ClauseVariable *left, ClauseVariable *right) {
    return left->isInteger() && right->isIdentifier();
}

bool EvaluatorUtils::SUtils::isValidIntegerSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isIntegerSynonym(left, right) && isVariable(right->getDesignEntityType());
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
           && isVariable(right->getDesignEntityType());
}

bool EvaluatorUtils::SUtils::isValidUsesSynonymSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymSynonym(left, right)
           && isUsesStmtType(left->getDesignEntityType())
           && isVariable(right->getDesignEntityType());
}


bool EvaluatorUtils::SUtils::isValidModifiesSynonymWildCard(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymWildCard(left, right) && isModifiesStmtType(left->getDesignEntityType());
}

bool EvaluatorUtils::SUtils::isValidUsesSynonymWildCard(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymWildCard(left, right) && isUsesStmtType(left->getDesignEntityType());
}

// ============================================ Calls Utils ====================================================

bool EvaluatorUtils::CallsUtils::isValidCallsIdentifierSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isIdentifierSynonym(left, right) && isProcedure(right->getDesignEntityType());
}

bool EvaluatorUtils::CallsUtils::isValidCallsSynonymIdentifier(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymIdentifier(left, right) && isProcedure(left->getDesignEntityType());
}

bool EvaluatorUtils::CallsUtils::isValidCallsSynonymSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymSynonym(left, right)
           && isProcedure(left->getDesignEntityType())
           && isProcedure(right->getDesignEntityType());
}

bool EvaluatorUtils::CallsUtils::isValidCallsSynonymWildCard(ClauseVariable *left, ClauseVariable *right) {
    return isSynonymWildCard(left, right) && isProcedure(left->getDesignEntityType());
}

bool EvaluatorUtils::CallsUtils::isValidCallsWildCardSynonym(ClauseVariable *left, ClauseVariable *right) {
    return isWildCardSynonym(left, right) && isProcedure(right->getDesignEntityType());
}

// ============================================ Attr Utils ====================================================

bool EvaluatorUtils::AttrUtils::isValidSelectable(Selectable *target) {
    return target->getType() != Selectable::ATTR_REF || isProcProcNameAttr(target) || isCallProcNameAttr(target) ||
           isReadVarNameAttr(target) || isPrintVarNameAttr(target) || isVarVarNameAttr(target) ||
           isConstantValueAttr(target) || isStmtStmtNumAttr(target);
}

bool EvaluatorUtils::AttrUtils::isProcProcNameAttr(Selectable *target) {
    return target->getAttr() == Selectable::PROC_NAME && EvaluatorUtils::isProcedure(target->getSynonym().type);
}

bool EvaluatorUtils::AttrUtils::isCallProcNameAttr(Selectable *target) {
    return target->getAttr() == Selectable::PROC_NAME && EvaluatorUtils::isCall(target->getSynonym().type);
}

bool EvaluatorUtils::AttrUtils::isReadVarNameAttr(Selectable *target) {
    return target->getAttr() == Selectable::VAR_NAME && EvaluatorUtils::isRead(target->getSynonym().type);
}

bool EvaluatorUtils::AttrUtils::isPrintVarNameAttr(Selectable *target) {
    return target->getAttr() == Selectable::VAR_NAME && EvaluatorUtils::isPrint(target->getSynonym().type);
}

bool EvaluatorUtils::AttrUtils::isVarVarNameAttr(Selectable *target) {
    return target->getAttr() == Selectable::VAR_NAME && EvaluatorUtils::isVariable(target->getSynonym().type);
}

bool EvaluatorUtils::AttrUtils::isConstantValueAttr(Selectable *target) {
    return target->getAttr() == Selectable::VALUE && EvaluatorUtils::isConstant(target->getSynonym().type);
}

bool EvaluatorUtils::AttrUtils::isStmtStmtNumAttr(Selectable *target) {
    return target->getAttr() == Selectable::STMT_NUM && EvaluatorUtils::isStmtType(target->getSynonym().type);
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
                                               QueryDeclaration::design_entity_type type) {

    size_t size = type == QueryDeclaration::design_entity_type::IF ? 2 : 1;

    if (patternVariables.size() < size) {
        return false;
    }

    for (auto patternVariable: patternVariables) {
        if (!patternVariable.isWildcard()) {
            return false;
        }
    }

    return true;
}

bool EvaluatorUtils::PatternUtils::isWildCardWildCards(ClauseVariable variable,
                                                       const vector<PatternVariable> &patternVariables,
                                                       QueryDeclaration::design_entity_type type) {

    return variable.isWildCard() && isWildCards(patternVariables, type);
}

bool EvaluatorUtils::PatternUtils::isIdentifierWildCards(ClauseVariable variable,
                                                         const vector<PatternVariable> &patternVariables,
                                                         QueryDeclaration::design_entity_type type) {
    return variable.isIdentifier() && isWildCards(patternVariables, type);
}

bool EvaluatorUtils::PatternUtils::isValidSynonymWildCards(ClauseVariable variable,
                                                           const vector<PatternVariable> &patternVariables,
                                                           QueryDeclaration::design_entity_type type) {

    return isVariableSynonym(&variable) && isWildCards(patternVariables, type);
}