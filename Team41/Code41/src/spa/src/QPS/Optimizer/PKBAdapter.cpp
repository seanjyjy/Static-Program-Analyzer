#include "PKBAdapter.h"

PKBAdapter::PKBAdapter(PKBManager *pkb): pkb(pkb) {
}

long long PKBAdapter::getRowCount(const ClauseVariable &synonym) {
    switch (synonym.getDesignEntityType()) {
        case QueryDeclaration::STMT:
            return pkb->getStatementCount();
        case QueryDeclaration::READ:
            return pkb->getReadCount();
        case QueryDeclaration::PRINT:
            return pkb->getPrintCount();
        case QueryDeclaration::CALL:
            return pkb->getCallCount();
        case QueryDeclaration::WHILE:
            return pkb->getWhileCount();
        case QueryDeclaration::IF:
            return pkb->getIfCount();
        case QueryDeclaration::ASSIGN:
            return pkb->getAssignCount();
        case QueryDeclaration::VARIABLE:
            return pkb->getVariableCount();
        case QueryDeclaration::CONSTANT:
            return pkb->getConstantCount();
        case QueryDeclaration::PROCEDURE:
            return pkb->getProcedureCount();
        default:
            return pkb->getStatementCount();
    }
}

long long PKBAdapter::getRowCount(const vector<ClauseVariable> &synonyms) {
    long long rows = 0;
    for (const ClauseVariable &cv: synonyms) rows *= getRowCount(cv);
    return rows;
}
