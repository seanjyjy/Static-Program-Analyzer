#pragma once

#include "PKB/Relations/ManyToMany.h"
#include "PKB/Relations/OneToOne.h"
#include <string>

using namespace std;

/**
 * A relational table that stores the Calls abstractions
 * - Calls relation calls Many-Many relationship mapping
 * - CallsT relation follows Many-Many relationship mapping
 */
class CallsTable {
private:
    // Calls : 'Calls' '(' entRef ',' entRef ')'
    ManyToMany<string, string> callsRelation;
    // CallsT : 'Calls\*' '(' entRef ',' entRef ')'
    ManyToMany<string, string> callsTRelation;
public:
    CallsTable();

    /**
     * Set caller calls callee relation in the table
     *
     * @param caller the caller procedure
     * @param callee the callee procedure
     * @throws domain_error if relation violates the table relational constraint
     * @throws domain_error if recursion is detected
     */
    void setCalls(const string& caller, const string& callee);

    /**
     * Set caller callsT callee relation in the table
     *
     * @param caller the caller procedure
     * @param callee the callee procedure
     * @throws domain_error if relation violates the table relational constraint
     * @throws domain_error if recursion is detected
     */
    void setCallsT(const string& caller, const string& callee);

    unordered_set<string> getProcsCalledBy(string caller); // Gets the proc that is called by this caller
    unordered_set<string> getProcsCalling(string callee); // Gets the proc that calls this proc
    vector<pair<string, string>> getCallsEntries(); // Gets list of caller-callee pair
    bool isCalls(string caller, string callee); // Checks if the caller calls the called proc

    unordered_set<string> getProcsCalledTBy(string caller); // Gets set of procs that are calledT by this caller
    unordered_set<string> getProcsCallingT(string callee); // Gets set of procs that callsT this proc
    vector<pair<string, string>> getCallsTEntries(); // Gets list of ancestor-descendant pair
    bool isCallsT(string caller, string callee); // Checks if the caller callsT the callee proc

    unordered_set<string> getProcsCallingSomeProc(); // Get set of procs calling some proc
    unordered_set<string> getProcsCalledBySomeProc(); // Get set of procs called by some proc
};
