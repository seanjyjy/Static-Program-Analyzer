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

    void setVarUsedInStmt(string stmtNum, string varName);
    set<string> getVarsUsedInStmt(string stmtNum);
    set<string> getStmtsUsingVar(string varName);
    set<pair<string, string>> getStmtsVarEntries();
    bool isUsesS(string stmtNum, string varName);

    void setVarUsedInProc(string procName, string varName);
    set<string> getVarsUsedInProc(string procName);
    set<string> getProcsUsingVar(string varName);
    set<pair<string, string>> getProcVarEntries();
    bool isUsesP(string procName, string varName);
};
