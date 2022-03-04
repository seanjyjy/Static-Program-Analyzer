#include "CallsTEvaluator.h"

Table *CallsTEvaluator::evaluate(QueryClause clause, PKBClient *pkb) {
    auto leftVariable = clause.getLeftClauseVariable();
    auto rightVariable = clause.getRightClauseVariable();

    if (EvaluatorUtils::ProcUtils::isIdentifierIdentifier(&leftVariable, &rightVariable)) {
        return evaluateIdentifierIdentifier(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsIdentifierSynonym(&leftVariable, &rightVariable)) {
        return evaluateIdentifierSynonym(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::ProcUtils::isIdentifierWildCard(&leftVariable, &rightVariable)) {
        return evaluateIdentifierWildCard(pkb, leftVariable);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsSynonymIdentifier(&leftVariable, &rightVariable)) {
        return evaluateSynonymIdentifier(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsSynonymSynonym(&leftVariable, &rightVariable)) {
        return evaluateSynonymSynonym(pkb, leftVariable, rightVariable);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsSynonymWildCard(&leftVariable, &rightVariable)) {
        return evaluateSynonymWildCard(pkb, leftVariable);
    }

    if (EvaluatorUtils::CallsUtils::isWildCardIdentifier(&leftVariable, &rightVariable)) {
        return evaluateWildCardIdentifier(pkb, rightVariable);
    }

    if (EvaluatorUtils::CallsUtils::isValidCallsWildCardSynonym(&leftVariable, &rightVariable)) {
        return evaluateWildCardSynonym(pkb, rightVariable);
    }

    if (EvaluatorUtils::isWildCardWildCard(&leftVariable, &rightVariable)) {
        return evaluateWildCardWildCard(pkb);
    }

    throw SemanticException("Invalid query provided for Call");
}

Table *CallsTEvaluator::evaluateIdentifierIdentifier(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    bool isCalls = pkb->isCallsT(left.getLabel(), right.getLabel());

    if (!isCalls) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *CallsTEvaluator::evaluateIdentifierSynonym(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> procCalledBy = pkb->getAllProcCalledTBy(left.getLabel());

    if (procCalledBy.empty()) {
        return new FalseTable();
    }

    string column = right.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& proc : procCalledBy) {
        Row* row = new Row(column, proc);
        table->addRow(row);
    }

    return table;
}

Table *CallsTEvaluator::evaluateIdentifierWildCard(PKBClient *pkb, ClauseVariable left) {
    unordered_set<string> procCalledBy = pkb->getAllProcCalledTBy(left.getLabel());

    if (procCalledBy.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *CallsTEvaluator::evaluateSynonymIdentifier(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    unordered_set<string> procCalling = pkb->getAllProcCallingT(right.getLabel());

    if (procCalling.empty()) {
        return new FalseTable();
    }

    string column = left.getLabel();
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& proc : procCalling) {
        Row* row = new Row(column, proc);
        table->addRow(row);
    }

    return table;
}

Table *CallsTEvaluator::evaluateSynonymSynonym(PKBClient *pkb, ClauseVariable left, ClauseVariable right) {
    vector<pair<string, string>> listOfProcToProc = pkb->getAllCallsT();

    if (listOfProcToProc.empty()) {
        return new FalseTable();
    }

    string firstColumn = left.getLabel();
    string secondColumn = right.getLabel();
    Header header = Header({firstColumn, secondColumn});
    Table* table = new PQLTable(header);

    for (auto& procToProc : listOfProcToProc) {
        Row* row = new Row();
        row->addEntry(firstColumn, procToProc.first);
        row->addEntry(secondColumn, procToProc.second);
        table->addRow(row);
    }

    return table;
}

Table *CallsTEvaluator::evaluateSynonymWildCard(PKBClient *pkb, ClauseVariable left) {
    // TODO
    return nullptr;
}

Table *CallsTEvaluator::evaluateWildCardIdentifier(PKBClient *pkb, ClauseVariable right) {
    unordered_set<string> procCalling = pkb->getAllProcCallingT(right.getLabel());

    if (procCalling.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *CallsTEvaluator::evaluateWildCardSynonym(PKBClient *pkb, ClauseVariable right) {
    // TODO
    return nullptr;
}

Table *CallsTEvaluator::evaluateWildCardWildCard(PKBClient *pkb) {
    vector<pair<string, string>> listOfProcToProc = pkb->getAllCallsT();

    if (listOfProcToProc.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}
