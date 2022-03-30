#include "WhileReader.h"

long long WhileReader::getRowcount(PKBClient *pkb) const {
    return (long long) pkb->getWhileCount();
}

unordered_set<string> WhileReader::getEntities(PKBClient *pkb) {
    return pkb->getWhiles();
}

WhileReader::~WhileReader() = default;