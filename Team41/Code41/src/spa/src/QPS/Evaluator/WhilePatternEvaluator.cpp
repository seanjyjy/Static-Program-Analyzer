#include "WhilePatternEvaluator.h"
#include "Common/TreeUtils.h"

Table *WhilePatternEvaluator::evaluate(PatternClause clause, PKBClient *pkb) {
    QueryDeclaration patternSynonym = clause.getSynonym();
    ClauseVariable leftVariable = clause.getLHS();
    PatternVariable rightVariable = clause.getRHS().at(0);

    if (leftVariable.isWildCard() && rightVariable.isWildcard()) {
        return evaluateWildCardWildCard(pkb, patternSynonym);
    }

    if (leftVariable.isIdentifier() && rightVariable.isWildcard()) {
        return evaluateIdentifierWildCard(pkb, patternSynonym, leftVariable);
    }

    if (EvaluatorUtils::isVariableSynonym(&leftVariable) && rightVariable.isWildcard()) {
        return evaluateSynonymWildCard(pkb, patternSynonym, leftVariable);
    }

    throw SemanticException("Invalid query provided for Pattern");
}

Table *WhilePatternEvaluator::evaluateWildCardWildCard(PKBClient *pkb, QueryDeclaration patternSyn) {
    // return a table with list of while statements
    unordered_set<string> setOfWhileStmt = pkb->getWhileStmtUsingSomeVarCond();

    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfWhileStmt) {
        Row *row = new Row(column, stmtNum);
        result->addRow(row);
    }

    return result;
}

Table *WhilePatternEvaluator::evaluateIdentifierWildCard(PKBClient *pkb, QueryDeclaration patternSyn, ClauseVariable left) {
    unordered_set<string> setOfWhileStmt = pkb->getWhileStmtUsingVarCond(left.getLabel());

    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfWhileStmt) {
        Row *row = new Row(column, stmtNum);
        result->addRow(row);
    }

    return result;
}

Table *WhilePatternEvaluator::evaluateSynonymWildCard(PKBClient *pkb, QueryDeclaration patternSyn, ClauseVariable left) {
    vector<pair<string, string>> listOfStmtNVar = pkb->getWhileStmtNVarPairs();

    string firstColumn = patternSyn.synonym;
    string secondColumn = left.getLabel();
    Header header = Header({firstColumn, secondColumn});
    Table *result = new PQLTable(header);

    for (auto &[stmt, var]: listOfStmtNVar) {
        Row* row = new Row();
        row->addEntry(firstColumn, stmt);
        row->addEntry(secondColumn, var);
        result->addRow(row);
    }
    return result;
}