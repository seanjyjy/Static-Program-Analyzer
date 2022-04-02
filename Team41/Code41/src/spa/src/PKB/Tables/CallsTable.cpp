#include "CallsTable.h"
#include <unordered_set>
#include <stdexcept>

CallsTable::CallsTable() : callsRelation("Calls"), callsTRelation("CallsT") {}

void CallsTable::setCalls(const string &caller, const string &callee) {
    if (caller == callee) { throw domain_error("[PKB][CallsTable] Recursion is not allowed"); }

    unordered_set<string> allCallee = getProcsCalledTBy(callee);
    if (allCallee.find(caller) != allCallee.end()) {
        throw domain_error("[PKB][CallsTable] Cyclic dependency detected in call graph");
    }

    callsRelation.addMapping(caller, callee);
}

void CallsTable::setCallsT(const string &caller, const string &callee) {
    if (caller == callee) { throw domain_error("[PKB][CallsTable] Recursion is not allowed"); }

    unordered_set<string> allCallee = getProcsCalledTBy(callee);
    if (allCallee.find(caller) != allCallee.end()) {
        throw domain_error("[PKB][CallsTable] Cyclic dependency detected in call graph");
    }

    callsTRelation.addMapping(caller, callee);
}

unordered_set<string> CallsTable::getProcsCalledBy(const string &caller) {
    return callsRelation.getValuesFromKey(caller);
}

unordered_set<string> CallsTable::getProcsCalling(const string &callee) {
    return callsRelation.getKeysFromValue(callee);
}

vector<pair<string, string>> CallsTable::getCallsEntries() {
    return callsRelation.getEntries();
}

bool CallsTable::isCalls(const string &caller, const string &callee) {
    return callsRelation.hasMapping(caller, callee);
}

unordered_set<string> CallsTable::getProcsCalledTBy(const string &caller) {
    return callsTRelation.getValuesFromKey(caller);
}

unordered_set<string> CallsTable::getProcsCallingT(const string &callee) {
    return callsTRelation.getKeysFromValue(callee);
}

vector<pair<string, string>> CallsTable::getCallsTEntries() {
    return callsTRelation.getEntries();
}

bool CallsTable::isCallsT(const string &caller, const string &callee) {
    return callsTRelation.hasMapping(caller, callee);
}

unordered_set<string> CallsTable::getProcsCalledBySomeProc() {
    return callsRelation.getValues();
}

unordered_set<string> CallsTable::getProcsCallingSomeProc() {
    return callsRelation.getKeys();
}
