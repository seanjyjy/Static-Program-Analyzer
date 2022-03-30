
#include "ReadReader.h"

long long ReadReader::getRowcount(PKBClient *pkb) const {
    return (long long) pkb->getReadCount();
}

unordered_set<string> ReadReader::getEntities(PKBClient *pkb) {
    return pkb->getReads();
}

ReadReader::~ReadReader() = default;