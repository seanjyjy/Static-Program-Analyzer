#include "PKBAdapter.h"

PKBAdapter::PKBAdapter() = default;

PKBAdapter::PKBAdapter(PKBManager *pkb): pkb(pkb) {
}

long long PKBAdapter::getRowCount(const QueryDeclaration &synonym) {
    switch (synonym.getType()) {
        case QueryDeclaration::STMT:
            return (long long) pkb->getStatementCount();
        case QueryDeclaration::READ:
            return (long long) pkb->getReadCount();
        case QueryDeclaration::PRINT:
            return (long long) pkb->getPrintCount();
        case QueryDeclaration::CALL:
            return (long long) pkb->getCallCount();
        case QueryDeclaration::WHILE:
            return (long long) pkb->getWhileCount();
        case QueryDeclaration::IF:
            return (long long) pkb->getIfCount();
        case QueryDeclaration::ASSIGN:
            return (long long) pkb->getAssignCount();
        case QueryDeclaration::VARIABLE:
            return (long long) pkb->getVariableCount();
        case QueryDeclaration::CONSTANT:
            return (long long) pkb->getConstantCount();
        case QueryDeclaration::PROCEDURE:
            return (long long) pkb->getProcedureCount();
        default:
            return (long long) pkb->getStatementCount();
    }
}

long long PKBAdapter::getRowCount(const vector<QueryDeclaration> &synonyms) {
    long long rows = 0;
    for (const QueryDeclaration &cv: synonyms) rows *= getRowCount(cv);
    return rows;
}
