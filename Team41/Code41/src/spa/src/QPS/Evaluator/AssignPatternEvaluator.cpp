#include "AssignPatternEvaluator.h"
#include "Common/TreeUtils.h"

AssignPatternEvaluator::AssignPatternEvaluator(PKBClient *pkb) : PatternEvaluator(pkb) {}

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

    if (leftVariable.isIdentifier() && rightVariable.isWildcard()) {
        return evaluateIdentifierWildCard(patternSynonym, leftVariable);
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

Table *AssignPatternEvaluator::evaluateWildCardFullPattern(QueryDeclaration patternSyn, PatternVariable right) {
    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromPattern(right.getMiniAST());
    return buildSingleSynonymTable(setOfAssignStmt, patternSyn);
}

Table *AssignPatternEvaluator::evaluateWildCardSubPattern(QueryDeclaration patternSyn, PatternVariable right) {
    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromSubpattern(right.getMiniAST());
    return buildSingleSynonymTable(setOfAssignStmt, patternSyn);
}

Table *AssignPatternEvaluator::evaluateIdentifierFullPattern(QueryDeclaration patternSyn, const ClauseVariable &left,
                                                             PatternVariable right) {
    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromPatternNVar(right.getMiniAST(), left.getLabel());
    return buildSingleSynonymTable(setOfAssignStmt, patternSyn);
}

Table *AssignPatternEvaluator::evaluateIdentifierSubPattern(QueryDeclaration patternSyn, const ClauseVariable &left,
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

Table *AssignPatternEvaluator::evaluateIdentifierWildCard(QueryDeclaration patternSynonym,
                                                          ClauseVariable &leftVariable) {
    unordered_set<string> setOfAssignStmt = pkb->getAssigns();
    return buildAssignPatternSTable(setOfAssignStmt, patternSynonym, leftVariable);
}

Table *AssignPatternEvaluator::buildAssignPatternSSTable(const vector<pair<string, string>> &results,
                                                         QueryDeclaration &patternSyn, ClauseVariable &variable) {
    if (results.empty()) {
        return new FalseTable();
    }

    string firstColumn = patternSyn.synonym;
    string secondColumn = variable.getLabel();
    unordered_set<string> leftFilters = getFilters(patternSyn.type);
    unordered_set<string> rightFilters = getFilters(variable.getDesignEntityType());

    Header header = Header({firstColumn, secondColumn});
    Table *table = new PQLTable(header);

    for (auto &[leftSyn, rightSyn]: results) {
        bool isInLeftFilter = leftFilters.find(leftSyn) != leftFilters.end();
        bool isInRightFilter = rightFilters.find(rightSyn) != rightFilters.end();
        if (isInLeftFilter && isInRightFilter) {
            Row *row = new Row();
            row->addEntry(firstColumn, leftSyn);
            row->addEntry(secondColumn, rightSyn);
            table->addRow(row);
        }
    }
    return table;
}

Table *AssignPatternEvaluator::buildAssignPatternSTable(const unordered_set<string> &results,
                                                        QueryDeclaration &patternSyn, ClauseVariable &variable) {
    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: results) {
        if (pkb->isModifiesS(stmtNum, variable.getLabel())) {
            Row *row = new Row(column, stmtNum);
            result->addRow(row);
        }
    }

    return result;
}


Table *AssignPatternEvaluator::buildAssignPatternSSTable(const unordered_set<string> &results,
                                                         QueryDeclaration &patternSyn, ClauseVariable &variable) {
    if (results.empty()) {
        return new FalseTable();
    }

    string firstColumn = patternSyn.synonym;
    string secondColumn = variable.getLabel();

    unordered_set<string> leftFilters = getFilters(patternSyn.type);
    unordered_set<string> rightFilters = getFilters(variable.getDesignEntityType());

    Header header = Header({firstColumn, secondColumn});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: results) {
        unordered_set<string> setOfAssignedVars = pkb->getModifiesByStmt(stmtNum);
        for (auto &varName: setOfAssignedVars) {
            // uniqueness guaranteed
            bool isInLeftFilter = leftFilters.find(stmtNum) != leftFilters.end();
            bool isInRightFilter = rightFilters.find(varName) != rightFilters.end();
            if (isInLeftFilter && isInRightFilter) {
                Row *row = new Row();
                row->addEntry(firstColumn, stmtNum);
                row->addEntry(secondColumn, varName);
                result->addRow(row);
            }
        }
    }

    return result;
}