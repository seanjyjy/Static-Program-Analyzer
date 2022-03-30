#include "VariableReader.h"

long long VariableReader::getRowcount(PKBClient *pkb) const {
    return (long long) pkb->getVariableCount();
}

unordered_set<string> VariableReader::getEntities(PKBClient *pkb) {
    return pkb->getVariables();
}

VariableReader::~VariableReader() = default;