#pragma once

#include "ManyToMany.h"
#include <string>

using namespace std;

class UsesTable {
private:
    // UsesS : 'Uses' '(' stmtRef ',' entRef ')'
    ManyToMany<string, string> usesStmtRelation;
    // UsesP : 'Uses' '(' entRef ',' entRef ')'
    ManyToMany<string, string> usesProcRelation;
public:
    UsesTable();

    void setVarUsedInStmt(const string& stmtNum, const string& varName);
    unordered_set<string> getVarsUsedInStmt(string stmtNum);
    unordered_set<string> getStmtsUsingVar(string varName);
    vector<pair<string, string>> getStmtsVarEntries();
    unordered_set<string> getVarsUsedInSomeStmt();
    unordered_set<string> getStmtsUsingSomeVar();
    bool isUsesS(string stmtNum, string varName);

    void setVarUsedInProc(const string& procName, const string& varName);
    unordered_set<string> getVarsUsedInProc(string procName);
    unordered_set<string> getProcsUsingVar(string varName);
    vector<pair<string, string>> getProcVarEntries();
    unordered_set<string> getVarsUsedInSomeProc();
    unordered_set<string> getProcsUsingSomeVar();
    bool isUsesP(string procName, string varName);
};
