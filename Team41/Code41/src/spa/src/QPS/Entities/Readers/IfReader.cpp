#include "IfReader.h"

long long IfReader::getRowcount(PKBClient *pkb) const {
    return (long long) pkb->getIfCount();
}

unordered_set<string> IfReader::getEntities(PKBClient *pkb) {
    return pkb->getIfs();
}

IfReader::~IfReader() = default;