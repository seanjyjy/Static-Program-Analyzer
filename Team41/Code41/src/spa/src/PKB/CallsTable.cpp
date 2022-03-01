#include "CallsTable.h"
#include <unordered_set>
#include <stdexcept>

CallsTable::CallsTable() : callsRelation("Calls"), callsTRelation("CallsT") {}

void CallsTable::setCalls(string caller, string callee) {
    if (caller == callee) { throw domain_error("[PKB][CallsTable] Recursion is not allowed"); }
    if (isCalls(caller, callee)) { return; }

    unordered_set<string> allCallee = getProcsCalledTBy(callee);
    if (allCallee.find(caller) != allCallee.end()) {
        throw domain_error("[PKB][CallsTable] Cyclic dependency detected in call graph");
    }

    callsRelation.addMapping(caller, callee);
}

void CallsTable::setCallsT(string caller, string callee) {
    if (caller == callee) { throw domain_error("[PKB][CallsTable] Recursion is not allowed"); }
    if (isCallsT(caller, callee)) { return; }

    unordered_set<string> allCallee = getProcsCalledTBy(callee);
    if (allCallee.find(caller) != allCallee.end()) {
        throw domain_error("[PKB][CallsTable] Cyclic dependency detected in call graph");
    }

    callsTRelation.addMapping(caller, callee);
}

unordered_set<string> CallsTable::getProcsCalledBy(string caller) {
    return callsRelation.getValuesFromKey(caller);
}

unordered_set<string> CallsTable::getProcsCalling(string callee) {
    return callsRelation.getKeysFromValue(callee);
}

vector<pair<string, string>> CallsTable::getCallsEntries() {
    return callsRelation.getEntries();
}

bool CallsTable::isCalls(string follower, string followed) {
    return callsRelation.hasMapping(follower, followed);
}

unordered_set<string> CallsTable::getProcsCalledTBy(string caller) {
    return callsTRelation.getValuesFromKey(caller);
}

unordered_set<string> CallsTable::getProcsCallingT(string callee) {
    return callsTRelation.getKeysFromValue(callee);
}

vector<pair<string, string>> CallsTable::getCallsTEntries() {
    return callsTRelation.getEntries();
}

bool CallsTable::isCallsT(string follower, string followed) {
    return callsTRelation.hasMapping(follower, followed);
}