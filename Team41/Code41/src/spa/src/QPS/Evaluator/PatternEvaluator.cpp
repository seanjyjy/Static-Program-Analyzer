#include "PatternEvaluator.h"
#include "Common/TreeUtils.h"

Table *PatternEvaluator::evaluate(PatternClause clause, PKB *pkb) {
    auto patternSynonym = clause.getSynonym();
    auto leftVariable = clause.getLHS();
    auto rightVariable = clause.getRHS();

    // check if leftVariable is variable type
    if (!EvaluatorUtils::isAssign(patternSynonym.type)) {
        return new FalseTable();
    }

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

Table *PatternEvaluator::evaluateWildCardFullPattern(PKB *pkb, QueryDeclaration patternSyn, PatternVariable right) {
    unordered_set<string> setOfAssignStmt = pkb->getAllStmtsFromFullPattern(right.getMiniAST());

    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfAssignStmt) {
        Row *row = new Row(column, stmtNum);
        result->addRow(row);
    }

    return result;
}

Table *PatternEvaluator::evaluateWildCardSubPattern(PKB *pkb, QueryDeclaration patternSyn, PatternVariable right) {
    unordered_set<string> setOfAssignStmt = pkb->getAllStmtsFromSubPattern(right.getMiniAST());

    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfAssignStmt) {
        Row *row = new Row(column, stmtNum);
        result->addRow(row);
    }

    return result;
}

Table *PatternEvaluator::evaluateWildCardWildCard(PKB *pkb, QueryDeclaration patternSyn) {
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

Table *PatternEvaluator::evaluateIdentifierFullPattern(PKB *pkb, QueryDeclaration patternSyn, ClauseVariable left,
                                                       PatternVariable right) {

    unordered_set<string> setOfAssignStmt = pkb->getStmtFromFullPatternNVar(right.getMiniAST(), left.getLabel());

    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfAssignStmt) {
        Row *row = new Row(column, stmtNum);
        result->addRow(row);
    }

    return result;
}

Table *PatternEvaluator::evaluateIdentifierSubPattern(PKB *pkb, QueryDeclaration patternSyn, ClauseVariable left,
                                                      PatternVariable right) {

    unordered_set<string> setOfAssignStmt = pkb->getStmtFromSubPatternNVar(right.getMiniAST(), left.getLabel());

    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfAssignStmt) {
        Row *row = new Row(column, stmtNum);
        result->addRow(row);
    }

    return result;
}

Table *PatternEvaluator::evaluateIdentifierWildCard(PKB *pkb, QueryDeclaration patternSyn, ClauseVariable left) {
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

Table *PatternEvaluator::evaluateSynonymFullPattern(PKB *pkb, QueryDeclaration patternSyn, ClauseVariable left,
                                                    PatternVariable right) {
    vector<pair<string, string>> listOfStmtNVar = pkb->getStmtNVarFromFullPattern(right.getMiniAST());

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

Table *PatternEvaluator::evaluateSynonymSubPattern(PKB *pkb, QueryDeclaration patternSyn, ClauseVariable left,
                                                   PatternVariable right) {
    vector<pair<string, string>> listOfStmtNVar = pkb->getStmtNVarFromSubPattern(right.getMiniAST());

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

Table *PatternEvaluator::evaluateSynonymWildCard(PKB *pkb, QueryDeclaration patternSyn, ClauseVariable left) {
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