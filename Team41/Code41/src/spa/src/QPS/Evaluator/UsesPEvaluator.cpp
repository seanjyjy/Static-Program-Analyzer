#include "UsesPEvaluator.h"

Table* UsesPEvaluator::evaluate(QueryClause clause, PKBClient* pkb) {
    auto leftVariable = clause.getLeftClauseVariable();
    auto rightVariable = clause.getRightClauseVariable();

    if (EvaluatorUtils::ProcUtils::isIdentifierIdentifier(&leftVariable, &rightVariable)) {
        return evaluateIdentifierIdentifier(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::ProcUtils::isValidIdentifierSynonym(&leftVariable, &rightVariable)) {
        return evaluateIdentifierSynonym(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::ProcUtils::isIdentifierWildCard(&leftVariable, &rightVariable)) {
        return evaluateIdentifierWildCard(pkb, leftVariable);
    }

    if (EvaluatorUtils::ProcUtils::isValidSynonymIdentifier(&leftVariable, &rightVariable)) {
        return evaluateSynonymIdentifier(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::ProcUtils::isValidSynonymSynonym(&leftVariable, &rightVariable)) {
        return evaluateSynonymSynonym(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::ProcUtils::isValidSynonymWildCard(&leftVariable, &rightVariable)) {
        return evaluateSynonymWildCard(pkb, leftVariable);
    }

    throw SemanticException("Invalid query provided for Uses");
}

Table* UsesPEvaluator::evaluateIdentifierIdentifier(PKBClient* pkb, ClauseVariable left, ClauseVariable right) {
    bool isUsesP = pkb->isUsesP(left.getLabel(), right.getLabel());

    if (isUsesP) {
        return new TrueTable();
    }

    return new FalseTable();
}

Table* UsesPEvaluator::evaluateIdentifierSynonym(PKBClient* pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfVariables = pkb->getUsesByProc(left.getLabel());

    string column = right.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& variable : setOfVariables) {
        Row* row = new Row(column, variable);
        result->addRow(row);
    }

    return result;
}

Table* UsesPEvaluator::evaluateIdentifierWildCard(PKBClient* pkb, ClauseVariable left) {
    unordered_set<string> setOfVariables = pkb->getUsesByProc(left.getLabel());

    if (setOfVariables.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table* UsesPEvaluator::evaluateSynonymIdentifier(PKBClient* pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> setOfProcedures = pkb->getUsesPByVar(right.getLabel());

    string column = left.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& procedure : setOfProcedures) {
        Row* row = new Row(column, procedure);
        result->addRow(row);
    }

    return result;
}

Table* UsesPEvaluator::evaluateSynonymSynonym(PKBClient* pkb, ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> listOfProcedureToVariable =  pkb->getAllUsesP();

    string firstColumn = left.getLabel();
    string secondColumn = right.getLabel();
    Header header = Header({firstColumn, secondColumn});
    Table* result = new PQLTable(header);

    for (auto& procedureToVariable : listOfProcedureToVariable) {
        Row* row = new Row();
        row->addEntry(firstColumn, procedureToVariable.first);
        row->addEntry(secondColumn, procedureToVariable.second);
        result->addRow(row);
    }

    return result;
}

Table* UsesPEvaluator::evaluateSynonymWildCard(PKBClient* pkb, ClauseVariable left) {
    unordered_set<string> setOfProcedures =  pkb->getAllProcsUsingSomeVar();

    string column = left.getLabel();
    Header header = Header({column});
    Table* result = new PQLTable(header);

    for (auto& procedure : setOfProcedures) {
        Row* row = new Row(column, procedure);
        result->addRow(row);
    }

    return result;
}
