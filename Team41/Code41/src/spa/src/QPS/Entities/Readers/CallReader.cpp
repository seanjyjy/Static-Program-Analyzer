#include "CallReader.h"

long long CallReader::getRowcount(PKBClient *pkb) const {
    return pkb->getCallCount();
}

unordered_set<string> CallReader::getEntities(PKBClient *pkb) {
    return pkb->getCalls();
}

CallReader::~CallReader() = default;
