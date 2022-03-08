#include "IfPatternEvaluator.h"
#include "Common/TreeUtils.h"

Table *IfPatternEvaluator::evaluate(PatternClause clause, PKBClient *pkb) {
    QueryDeclaration patternSynonym = clause.getSynonym();
    ClauseVariable leftVariable = clause.getLHS();
    PatternVariable thenPattern = clause.getRHS().at(0);
    PatternVariable elsePattern = clause.getRHS().at(1);

    if (leftVariable.isWildCard() && thenPattern.isWildcard() && elsePattern.isWildcard()) {
        return evaluateWildCardWildCard(pkb, patternSynonym);
    }

    if (leftVariable.isIdentifier() && thenPattern.isWildcard() && elsePattern.isWildcard()) {
        return evaluateIdentifierWildCard(pkb, patternSynonym, leftVariable);
    }

    if (EvaluatorUtils::isVariableSynonym(&leftVariable) && thenPattern.isWildcard() && elsePattern.isWildcard()) {
        return evaluateSynonymWildCard(pkb, patternSynonym, leftVariable);
    }

    throw SemanticException("Invalid query provided for Pattern");
}

Table *IfPatternEvaluator::evaluateWildCardWildCard(PKBClient *pkb, QueryDeclaration patternSyn) {
    // return a table with list of if statements
    unordered_set<string> setOfIfStmt = pkb->getIfStmtUsingSomeVarCond();

    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfIfStmt) {
        Row *row = new Row(column, stmtNum);
        result->addRow(row);
    }

    return result;
}

Table *IfPatternEvaluator::evaluateIdentifierWildCard(PKBClient *pkb, QueryDeclaration patternSyn, ClauseVariable left) {
    unordered_set<string> setOfIfStmt = pkb->getIfStmtUsingVarCond(left.getLabel());

    string column = patternSyn.synonym;
    Header header = Header({column});
    Table *result = new PQLTable(header);

    for (auto &stmtNum: setOfIfStmt) {
            Row *row = new Row(column, stmtNum);
            result->addRow(row);
    }

    return result;
}

Table *IfPatternEvaluator::evaluateSynonymWildCard(PKBClient *pkb, QueryDeclaration patternSyn, ClauseVariable left) {
    vector<pair<string, string>> listOfStmtNVar = pkb->getIfStmtNVarPairs();

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