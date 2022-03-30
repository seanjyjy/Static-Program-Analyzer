#include "ConstantReader.h"

long long ConstantReader::getRowcount(PKBClient *pkb) const {
    return (long long) pkb->getConstantCount();
}

unordered_set<string> ConstantReader::getEntities(PKBClient *pkb) {
    return pkb->getConstants();
}

ConstantReader::~ConstantReader() = default;