#include "AssignPatternEvaluator.h"
#include "Common/TreeUtils.h"

AssignPatternEvaluator::AssignPatternEvaluator(PKBClient *pkb): PatternEvaluator(pkb) {}

Table *AssignPatternEvaluator::evaluateFurther(QueryDeclaration patternSynonym, ClauseVariable &leftVariable,
                                               vector<PatternVariable> &rightPatternVariables) {

    PatternVariable rightVariable = rightPatternVariables.at(0);
    if (leftVariable.isWildCard() && rightVariable.isFullPattern()) {
        return evaluateWildCardFullPattern(patternSynonym, rightVariable);
    }

    if (leftVariable.isWildCard() && rightVariable.isSubPattern()) {
        return evaluateWildCardSubPattern(patternSynonym, rightVariable);
    }

    if (leftVariable.isIdentifier() && rightVariable.isFullPattern()) {
        return evaluateIdentifierFullPattern(patternSynonym, leftVariable, rightVariable);
    }

    if (leftVariable.isIdentifier() && rightVariable.isSubPattern()) {
        return evaluateIdentifierSubPattern(patternSynonym, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::isVariableSynonym(&leftVariable) && rightVariable.isFullPattern()) {
        return evaluateSynonymFullPattern(patternSynonym, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::isVariableSynonym(&leftVariable) && rightVariable.isSubPattern()) {
        return evaluateSynonymSubPattern(patternSynonym, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::isVariableSynonym(&leftVariable) && rightVariable.isWildcard()) {
        return evaluateSynonymWildCard(patternSynonym, leftVariable);
    }

    throw SemanticException("Invalid query provided for Pattern");
}

unordered_set<string> AssignPatternEvaluator::getWildCardWildCardRelation() {
    return pkb->getAssigns();
}

unordered_set<string> AssignPatternEvaluator::getIdentifierWildCardRelation(const string &label) {
    return pkb->getAssigns();
}


Table *AssignPatternEvaluator::evaluateWildCardFullPattern(QueryDeclaration patternSyn, PatternVariable right) {
    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromPattern(right.getMiniAST());
    return buildSingleSynonymTable(setOfAssignStmt, patternSyn);
}

Table *AssignPatternEvaluator::evaluateWildCardSubPattern(QueryDeclaration patternSyn, PatternVariable right) {
    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromSubpattern(right.getMiniAST());
    return buildSingleSynonymTable(setOfAssignStmt, patternSyn);
}

Table *AssignPatternEvaluator::evaluateIdentifierFullPattern(QueryDeclaration patternSyn, ClauseVariable left,
                                                             PatternVariable right) {
    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromPatternNVar(right.getMiniAST(), left.getLabel());
    return buildSingleSynonymTable(setOfAssignStmt, patternSyn);
}

Table *AssignPatternEvaluator::evaluateIdentifierSubPattern(QueryDeclaration patternSyn, ClauseVariable left,
                                                            PatternVariable right) {

    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromSubpatternNVar(right.getMiniAST(), left.getLabel());
    return buildSingleSynonymTable(setOfAssignStmt, patternSyn);
}

Table *AssignPatternEvaluator::evaluateSynonymFullPattern(QueryDeclaration patternSyn, ClauseVariable left,
                                                          PatternVariable right) {
    vector<pair<string, string>> listOfStmtNVar = pkb->getAssignStmtNVarFromPattern(right.getMiniAST());

    return buildAssignPatternSSTable(listOfStmtNVar, patternSyn, left);
}

Table *AssignPatternEvaluator::evaluateSynonymSubPattern(QueryDeclaration patternSyn, ClauseVariable left,
                                                         PatternVariable right) {
    vector<pair<string, string>> listOfStmtNVar = pkb->getAssignStmtNVarFromSubpattern(right.getMiniAST());
    return buildAssignPatternSSTable(listOfStmtNVar, patternSyn, left);
}

Table *AssignPatternEvaluator::evaluateSynonymWildCard(QueryDeclaration patternSyn, ClauseVariable left) {
    unordered_set<string> setOfAssignStmt = pkb->getAssigns();
    return buildAssignPatternSSTable(setOfAssignStmt, patternSyn, left);
}