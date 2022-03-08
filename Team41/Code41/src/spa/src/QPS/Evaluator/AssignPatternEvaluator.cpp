#include "AssignPatternEvaluator.h"
#include "Common/TreeUtils.h"

Table *AssignPatternEvaluator::evaluate(PatternClause clause, PKBClient *pkb) {
    auto patternSynonym = clause.getSynonym();
    auto leftVariable = clause.getLHS();
    auto rightVariable = clause.getRHS();

    if (leftVariable.isWildCard() && rightVariable.isFullPattern()) {
        return evaluateWildCardFullPattern(pkb, patternSynonym, rightVariable);
    }

    if (leftVariable.isWildCard() && rightVariable.isSubPattern()) {
        return evaluateWildCardSubPattern(pkb, patternSynonym, rightVariable);
    }

    if (leftVariable.isWildCard() && rightVariable.isWildcard()) {
        return evaluateWildCardWildCard(pkb, patternSynonym);
    }

    if (leftVariable.isIdentifier() && rightVariable.isFullPattern()) {
        return evaluateIdentifierFullPattern(pkb, patternSynonym, leftVariable, rightVariable);
    }

    if (leftVariable.isIdentifier() && rightVariable.isSubPattern()) {
        return evaluateIdentifierSubPattern(pkb, patternSynonym, leftVariable, rightVariable);
    }

    if (leftVariable.isIdentifier() && rightVariable.isWildcard()) {
        return evaluateIdentifierWildCard(pkb, patternSynonym, leftVariable);
    }

    if (EvaluatorUtils::isVariableSynonym(&leftVariable) && rightVariable.isFullPattern()) {
        return evaluateSynonymFullPattern(pkb, patternSynonym, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::isVariableSynonym(&leftVariable) && rightVariable.isSubPattern()) {
        return evaluateSynonymSubPattern(pkb, patternSynonym, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::isVariableSynonym(&leftVariable) && rightVariable.isWildcard()) {
        return evaluateSynonymWildCard(pkb, patternSynonym, leftVariable);
    }

    throw SemanticException("Invalid query provided for Pattern");
}

Table *AssignPatternEvaluator::evaluateWildCardFullPattern(PKBClient *pkb, QueryDeclaration patternSyn, PatternVariable right) {
    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromPattern(right.getMiniAST());

    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfAssignStmt) {
        Row *row = new Row(column, stmtNum);
        result->addRow(row);
    }

    return result;
}

Table *AssignPatternEvaluator::evaluateWildCardSubPattern(PKBClient *pkb, QueryDeclaration patternSyn, PatternVariable right) {
    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromSubpattern(right.getMiniAST());

    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfAssignStmt) {
        Row *row = new Row(column, stmtNum);
        result->addRow(row);
    }

    return result;
}

Table *AssignPatternEvaluator::evaluateWildCardWildCard(PKBClient *pkb, QueryDeclaration patternSyn) {
    // return a table with list of assignment statements
    unordered_set<string> setOfAssignStmt = pkb->getAssigns();

    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfAssignStmt) {
        Row *row = new Row(column, stmtNum);
        result->addRow(row);
    }

    return result;
}

Table *AssignPatternEvaluator::evaluateIdentifierFullPattern(PKBClient *pkb, QueryDeclaration patternSyn, ClauseVariable left,
                                                             PatternVariable right) {

    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromPatternNVar(right.getMiniAST(), left.getLabel());

    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfAssignStmt) {
        Row *row = new Row(column, stmtNum);
        result->addRow(row);
    }

    return result;
}

Table *AssignPatternEvaluator::evaluateIdentifierSubPattern(PKBClient *pkb, QueryDeclaration patternSyn, ClauseVariable left,
                                                            PatternVariable right) {

    unordered_set<string> setOfAssignStmt = pkb->getAssignStmtFromSubpatternNVar(right.getMiniAST(), left.getLabel());

    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfAssignStmt) {
        Row *row = new Row(column, stmtNum);
        result->addRow(row);
    }

    return result;
}

Table *AssignPatternEvaluator::evaluateIdentifierWildCard(PKBClient *pkb, QueryDeclaration patternSyn, ClauseVariable left) {
    unordered_set<string> setOfAssignStmt = pkb->getAssigns();

    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfAssignStmt) {
        if (pkb->isModifiesS(stmtNum, left.getLabel())) {
            Row *row = new Row(column, stmtNum);
            result->addRow(row);
        }
    }

    return result;
}

Table *AssignPatternEvaluator::evaluateSynonymFullPattern(PKBClient *pkb, QueryDeclaration patternSyn, ClauseVariable left,
                                                          PatternVariable right) {
    vector<pair<string, string>> listOfStmtNVar = pkb->getAssignStmtNVarFromPattern(right.getMiniAST());

    string firstColumn = patternSyn.synonym;
    string secondColumn = left.getLabel();
    Header header = Header({firstColumn, secondColumn});
    Table *result = new PQLTable(header);

    for (auto &stmtNVar: listOfStmtNVar) {
        Row* row = new Row();
        row->addEntry(firstColumn, stmtNVar.first);
        row->addEntry(secondColumn, stmtNVar.second);
        result->addRow(row);
    }
    return result;
}

Table *AssignPatternEvaluator::evaluateSynonymSubPattern(PKBClient *pkb, QueryDeclaration patternSyn, ClauseVariable left,
                                                         PatternVariable right) {
    vector<pair<string, string>> listOfStmtNVar = pkb->getAssignStmtNVarFromSubpattern(right.getMiniAST());

    string firstColumn = patternSyn.synonym;
    string secondColumn = left.getLabel();
    Header header = Header({firstColumn, secondColumn});
    Table *result = new PQLTable(header);

    for (auto &stmtNVar: listOfStmtNVar) {
        Row* row = new Row();
        row->addEntry(firstColumn, stmtNVar.first);
        row->addEntry(secondColumn, stmtNVar.second);
        result->addRow(row);
    }
    return result;

}

Table *AssignPatternEvaluator::evaluateSynonymWildCard(PKBClient *pkb, QueryDeclaration patternSyn, ClauseVariable left) {
    unordered_set<string> setOfAssignStmt = pkb->getAssigns();

    string firstColumn = patternSyn.synonym;
    string secondColumn = left.getLabel();
    Header header = Header({firstColumn, secondColumn});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfAssignStmt) {
        unordered_set<string> setOfAssignedVars = pkb->getModifiesByStmt(stmtNum);
        for (auto &varName: setOfAssignedVars) {
            // uniqueness guaranteed
            Row* row = new Row();
            row->addEntry(firstColumn, stmtNum);
            row->addEntry(secondColumn, varName);
            result->addRow(row);
        }
    }

    return result;
}