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

    void setVarModifiedInStmt(const string& stmtNum, const string& varName);
    unordered_set<string> getVarsModifiedInStmt(string stmtNum);
    unordered_set<string> getStmtsModifyingVar(string varName);
    vector<pair<string, string>> getStmtsVarEntries();
    unordered_set<string> getVarsModifiedInSomeStmt();
    unordered_set<string> getStmtsModifyingSomeVar();
    bool isModifiesS(string stmtNum, string varName);

    void setVarModifiedInProc(const string& procName, const string& varName);
    unordered_set<string> getVarsModifiedInProc(string procName);
    unordered_set<string> getProcsModifyingVar(string varName);
    vector<pair<string, string>> getProcVarEntries();
    unordered_set<string> getVarsModifiedInSomeProc();
    unordered_set<string> getProcsModifyingSomeVar();
    bool isModifiesP(string procName, string varName);
};
