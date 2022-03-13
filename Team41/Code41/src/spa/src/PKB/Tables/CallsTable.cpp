#include "CallsTable.h"
#include <unordered_set>
#include <stdexcept>

CallsTable::CallsTable() : callsRelation("Calls"), callsTRelation("CallsT") {}

void CallsTable::setCalls(const string& caller, const string& callee) {
    if (caller == callee) { throw domain_error("[PKB][CallsTable] Recursion is not allowed"); }
    if (isCalls(caller, callee)) { return; }

    unordered_set<string> allCallee = getProcsCalledTBy(callee);
    if (allCallee.find(caller) != allCallee.end()) {
        throw domain_error("[PKB][CallsTable] Cyclic dependency detected in call graph");
    }

    callsRelation.addMapping(caller, callee);
}

void CallsTable::setCallsT(const string& caller, const string& callee) {
    if (caller == callee) { throw domain_error("[PKB][CallsTable] Recursion is not allowed"); }
    if (isCallsT(caller, callee)) { return; }

    unordered_set<string> allCallee = getProcsCalledTBy(callee);
    if (allCallee.find(caller) != allCallee.end()) {
        throw domain_error("[PKB][CallsTable] Cyclic dependency detected in call graph");
    }

    callsTRelation.addMapping(caller, callee);
}

unordered_set<string> CallsTable::getProcsCalledBy(string caller) {
    return callsRelation.getValuesFromKey(move(caller));
}

unordered_set<string> CallsTable::getProcsCalling(string callee) {
    return callsRelation.getKeysFromValue(move(callee));
}

vector<pair<string, string>> CallsTable::getCallsEntries() {
    return callsRelation.getEntries();
}

bool CallsTable::isCalls(string caller, string callee) {
    return callsRelation.hasMapping(move(caller), move(callee));
}

unordered_set<string> CallsTable::getProcsCalledTBy(string caller) {
    return callsTRelation.getValuesFromKey(move(caller));
}

unordered_set<string> CallsTable::getProcsCallingT(string callee) {
    return callsTRelation.getKeysFromValue(move(callee));
}

vector<pair<string, string>> CallsTable::getCallsTEntries() {
    return callsTRelation.getEntries();
}

bool CallsTable::isCallsT(string caller, string callee) {
    return callsTRelation.hasMapping(move(caller), move(callee));
}

unordered_set<string> CallsTable::getProcsCalledBySomeProc() {
    return callsRelation.getValues();
}

unordered_set<string> CallsTable::getProcsCallingSomeProc() {
    return callsRelation.getKeys();
}
