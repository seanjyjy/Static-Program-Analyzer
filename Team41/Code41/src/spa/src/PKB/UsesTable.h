#pragma once

#include "ManyToMany.h"
#include <string>

using namespace std;

/**
 * A relational table that stores the Uses abstractions
 * - UsesS relation follows Many-Many relationship mapping
 * - UsesP relation follows Many-Many relationship mapping
 */
class UsesTable {
private:
    // UsesS : 'Uses' '(' stmtRef ',' entRef ')'
    ManyToMany<string, string> usesStmtRelation;
    // UsesP : 'Uses' '(' entRef ',' entRef ')'
    ManyToMany<string, string> usesProcRelation;
public:
    UsesTable();

    /**
     * Set stmt uses var relation in the table
     *
     * @param stmtNum the using statement
     * @param varName the used variable
     */
    void setVarUsedInStmt(const string& stmtNum, const string& varName);
    unordered_set<string> getVarsUsedInStmt(string stmtNum);// Gets set of variables that is used by the statement
    unordered_set<string> getStmtsUsingVar(string varName);// Gets set of statements that uses the variable
    vector<pair<string, string>> getStmtsVarEntries();// Gets list of stmt-var pairs where stmt uses var
    unordered_set<string> getVarsUsedInSomeStmt();// Gets set of vars where the var is used in some stmt
    unordered_set<string> getStmtsUsingSomeVar();// Gets set of stmts where the stmt uses some var
    bool isUsesS(string stmtNum, string varName);// Checks if the statement uses the variable

    /**
     * Set procedure uses var relation in the table
     *
     * @param procName the using procedure
     * @param varName the used variable
     */
    void setVarUsedInProc(const string& procName, const string& varName);
    unordered_set<string> getVarsUsedInProc(string procName);// Gets set of variables that is used by the procedure
    unordered_set<string> getProcsUsingVar(string varName);// Gets set of procedures that uses the variable
    vector<pair<string, string>> getProcVarEntries();// Gets list of proc-var pairs where procedure uses var
    unordered_set<string> getVarsUsedInSomeProc();// Gets set of vars where the var is used in some procedure
    unordered_set<string> getProcsUsingSomeVar();// Gets set of procedures where the procedure uses some var
    bool isUsesP(string procName, string varName);// Checks if the procedure uses the variable
};
