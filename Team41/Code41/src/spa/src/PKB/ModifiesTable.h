#pragma once

#include "ManyToMany.h"
#include <string>

using namespace std;

class ModifiesTable {
private:
    // ModifiesS : 'Modifies' '(' stmtRef ',' entRef ')'
    ManyToMany<string, string> modifyStmtRelation;
    // ModifiesP : 'Modifies' '(' entRef ',' entRef ')'
    ManyToMany<string, string> modifyProcRelation;
public:
    ModifiesTable();

    void setVarModifiedInStmt(string stmtNum, string varName);
    unordered_set<string> getVarsModifiedInStmt(string stmtNum);
    unordered_set<string> getStmtsModifyingVar(string varName);
    vector<pair<string, string>> getStmtsVarEntries();
    bool isModifiesS(string stmtNum, string varName);

    void setVarModifiedInProc(string procName, string varName);
    unordered_set<string> getVarsModifiedInProc(string procName);
    unordered_set<string> getProcsModifyingVar(string varName);
    vector<pair<string, string>> getProcVarEntries();
    bool isModifiesP(string procName, string varName);
};
