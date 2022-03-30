#include "ProcedureReader.h"

long long ProcedureReader::getRowcount(PKBClient *pkb) const {
    return (long long) pkb->getProcedureCount();
}

unordered_set<string> ProcedureReader::getEntities(PKBClient *pkb) {
    return pkb->getProcedures();
}

ProcedureReader::~ProcedureReader() = default;