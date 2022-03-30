#include "StmtReader.h"


long long StmtReader::getRowcount(PKBClient *pkb) const {
    return (long long) pkb->getStatementCount();
}

unordered_set<string> StmtReader::getEntities(PKBClient *pkb) {
    return pkb->getStatements();
}

StmtReader::~StmtReader() = default;