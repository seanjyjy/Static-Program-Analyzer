//
// Created by JinHao on 27/1/22.
//
#pragma once

#ifndef SPA_MODIFIESTABLE_H
#define SPA_MODIFIESTABLE_H

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
    set<string> getVarsModifiedInStmt(string stmtNum);
    set<string> getStmtsModifyingVar(string varName);
    set<pair<string, string>> getStmtsVarEntries();
    bool isModifiesS(string stmtNum, string varName);

    void setVarModifiedInProc(string procName, string varName);
    set<string> getVarsModifiedInProc(string procName);
    set<string> getProcsModifyingVar(string varName);
    set<pair<string, string>> getProcVarEntries();
    bool isModifiesP(string procName, string varName);
};


#endif //SPA_MODIFIESTABLE_H
