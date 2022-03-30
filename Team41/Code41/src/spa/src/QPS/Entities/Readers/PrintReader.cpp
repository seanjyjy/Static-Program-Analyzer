#include "PrintReader.h"

long long PrintReader::getRowcount(PKBClient *pkb) const {
    return (long long) pkb->getPrintCount();
}

unordered_set<string> PrintReader::getEntities(PKBClient *pkb) {
    return pkb->getPrints();
}

PrintReader::~PrintReader() = default;