#pragma once

#include "PKB/Relations/ManyToMany.h"
#include <string>

using namespace std;

/**
 * A relational table that stores the Modifies abstractions
 * - ModifiesS relation follows Many-Many relationship mapping
 * - ModifiesP relation follows Many-Many relationship mapping
 */
class ModifiesTable {
private:
    // ModifiesS : 'Modifies' '(' stmtRef ',' entRef ')'
    ManyToMany<string, string> modifyStmtRelation;
    // ModifiesP : 'Modifies' '(' entRef ',' entRef ')'
    ManyToMany<string, string> modifyProcRelation;
public:
    ModifiesTable();

    /**
     * Set stmt modifies var relation in the table
     *
     * @param stmtNum the modifying statement
     * @param varName the modified variable
     */
    void setVarModifiedInStmt(const string &stmtNum, const string &varName);

    unordered_set<string>
    getVarsModifiedInStmt(const string &stmtNum); // Gets set of variables that is modified by statement
    unordered_set<string> getStmtsModifyingVar(const string &varName); // Gets set of statements that modifies variable
    vector<pair<string, string>> getStmtsVarEntries(); // Gets list of stmt-var pair where stmt modifies var
    unordered_set<string> getVarsModifiedInSomeStmt(); // Gets set of var where var is modified by some stmt
    unordered_set<string> getStmtsModifyingSomeVar(); // Gets set of stmt where stmt modifies some var
    bool isModifiesS(string stmtNum, string varName); // Checks if statement modifies variable

    /**
     * Set procedure modifies var relation in the table
     *
     * @param procName the modifying procedure
     * @param varName the modified variable
     */
    void setVarModifiedInProc(const string &procName, const string &varName);

    unordered_set<string> getVarsModifiedInProc(const string &procName);// Gets variables that are modified by procedure
    unordered_set<string> getProcsModifyingVar(const string &varName);// Gets procedures that modify specified var
    vector<pair<string, string>> getProcVarEntries(); // Gets list of proc-var pair where proc modifies var
    unordered_set<string> getVarsModifiedInSomeProc(); // Gets set of var where var is modified by some proc
    unordered_set<string> getProcsModifyingSomeVar(); // Gets set of proc where proc modifies some var
    bool isModifiesP(const string &procName, const string &varName);// Checks if procedure modifies variable
};

