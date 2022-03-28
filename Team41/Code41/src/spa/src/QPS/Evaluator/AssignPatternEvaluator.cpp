#include "AssignPatternEvaluator.h"
#include "Common/TreeUtils.h"

AssignPatternEvaluator::AssignPatternEvaluator(PKBClient *pkb) : PatternEvaluator(pkb) {}

Table *AssignPatternEvaluator::evaluateFurther(QueryDeclaration &patternSynonym, ClauseVariable &leftVariable,
                                               vector<PatternVariable> &rightPatternVariables, bool canSimplify) {

    PatternVariable rightVariable = rightPatternVariables.at(0);
    if (leftVariable.isWildCard() && rightVariable.isFullPattern()) {
        return evaluateWildCardFullPattern(patternSynonym, rightVariable, canSimplify);
    }

    if (leftVariable.isWildCard() && rightVariable.isSubPattern()) {
        return evaluateWildCardSubPattern(patternSynonym, rightVariable, canSimplify);
    }

    if (leftVariable.isIdentifier() && rightVariable.isFullPattern()) {
        return evaluateIdentifierFullPattern(patternSynonym, leftVariable, rightVariable, canSimplify);
    }

    if (leftVariable.isIdentifier() && rightVariable.isSubPattern()) {
        return evaluateIdentifierSubPattern(patternSynonym, leftVariable, rightVariable, canSimplify);
    }

    if (leftVariable.isIdentifier() && rightVariable.isWildcard()) {
        return evaluateIdentifierWildCard(patternSynonym, leftVariable, canSimplify);
    }

    if (EvaluatorUtils::isVariableSynonym(&leftVariable) && rightVariable.isFullPattern()) {
        return evaluateSynonymFullPattern(patternSynonym, leftVariable, rightVariable, canSimplify);
    }

    if (EvaluatorUtils::isVariableSynonym(&leftVariable) && rightVariable.isSubPattern()) {
        return evaluateSynonymSubPattern(patternSynonym, leftVariable, rightVariable, canSimplify);
    }

    if (EvaluatorUtils::isVariableSynonym(&leftVariable) && rightVariable.isWildcard()) {
        return evaluateSynonymWildCard(patternSynonym, leftVariable, canSimplify);
    }

    throw SemanticException("Invalid query provided for Pattern");
}

unordered_set<string> AssignPatternEvaluator::getWildCardWildCardRelation() {
    return pkb->getAssigns();
}

Table *AssignPatternEvaluator::evaluateWildCardFullPattern(QueryDeclaration &patternSyn, PatternVariable &right, bool canSimplify) {
    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromPattern(right.getMiniAST());
    if (canSimplify) {
        return buildBooleanTable(!setOfAssignStmt.empty());
    }
    return buildSingleSynonymTable(setOfAssignStmt, patternSyn);
}

Table *AssignPatternEvaluator::evaluateWildCardSubPattern(QueryDeclaration &patternSyn, PatternVariable &right, bool canSimplify) {
    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromSubpattern(right.getMiniAST());
    if (canSimplify) {
        return buildBooleanTable(!setOfAssignStmt.empty());
    }
    return buildSingleSynonymTable(setOfAssignStmt, patternSyn);
}

Table *AssignPatternEvaluator::evaluateIdentifierFullPattern(QueryDeclaration &patternSyn, const ClauseVariable &left,
                                                             PatternVariable &right, bool canSimplify) {
    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromPatternNVar(right.getMiniAST(), left.getLabel());
    if (canSimplify) {
        return buildBooleanTable(!setOfAssignStmt.empty());
    }
    return buildSingleSynonymTable(setOfAssignStmt, patternSyn);
}

Table *AssignPatternEvaluator::evaluateIdentifierSubPattern(QueryDeclaration &patternSyn, const ClauseVariable &left,
                                                            PatternVariable &right, bool canSimplify) {
    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromSubpatternNVar(right.getMiniAST(), left.getLabel());
    if (canSimplify) {
        return buildBooleanTable(!setOfAssignStmt.empty());
    }
    return buildSingleSynonymTable(setOfAssignStmt, patternSyn);
}

Table *AssignPatternEvaluator::evaluateSynonymFullPattern(QueryDeclaration &patternSyn, ClauseVariable &left,
                                                          PatternVariable &right, bool canSimplify) {
    vector<pair<string, string>> listOfStmtNVar = pkb->getAssignStmtNVarFromPattern(right.getMiniAST());
    if (canSimplify) {
        return buildBooleanTable(!listOfStmtNVar.empty());
    }

    return buildSynonymSynonymPatternTable(listOfStmtNVar, patternSyn, left);
}

Table *AssignPatternEvaluator::evaluateSynonymSubPattern(QueryDeclaration &patternSyn, ClauseVariable &left,
                                                         PatternVariable &right, bool canSimplify) {
    vector<pair<string, string>> listOfStmtNVar = pkb->getAssignStmtNVarFromSubpattern(right.getMiniAST());
    if (canSimplify) {
        return buildBooleanTable(!listOfStmtNVar.empty());
    }
    return buildSynonymSynonymPatternTable(listOfStmtNVar, patternSyn, left);
}

Table *AssignPatternEvaluator::evaluateSynonymWildCard(QueryDeclaration &patternSyn, ClauseVariable &left, bool canSimplify) {
    vector<pair<string, string>> setOfStmtVar = pkb->getAllModifiesS();
    if (canSimplify) {
        return buildBooleanTable(!setOfStmtVar.empty());
    }
    return buildSynonymSynonymPatternTable(setOfStmtVar, patternSyn, left);
}

Table *AssignPatternEvaluator::evaluateIdentifierWildCard(QueryDeclaration &patternSynonym,
                                                          ClauseVariable &leftVariable, bool canSimplify) {
    unordered_set<string> setOfModifiedStmt = pkb->getModifiesSByVar(leftVariable.getLabel());
    if (canSimplify) {
        return buildBooleanTable(!setOfModifiedStmt.empty());
    }
    return buildSingleSynonymTable(setOfModifiedStmt, patternSynonym);
}
