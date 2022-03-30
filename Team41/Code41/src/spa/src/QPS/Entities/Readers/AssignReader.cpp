#include "AssignReader.h"

long long AssignReader::getRowcount(PKBClient *pkb) const {
    return (long long) pkb->getAssignCount();
}

unordered_set<string> AssignReader::getEntities(PKBClient *pkb) {
    return pkb->getAssigns();
}

AssignReader::~AssignReader() = default;