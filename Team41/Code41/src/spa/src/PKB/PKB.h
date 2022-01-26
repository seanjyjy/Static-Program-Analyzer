#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "StmtTable.h"

using namespace std;

class StmtTable;  // no need to #include "StmtTable.h" as all I need is pointer

class PKB {
public:
    PKB();
	StmtTable* stmtTable;

    /**
     * Gets set of statement that belongs to the specified type
     *
     * @param type the target SIMPLE statement type
     * @return the set of numeric string representing the stmt numbers
     */
    set<string> getStatements(StmtType type);

    /**
     * Gets all the statement type of the specified statement number
     *
     * @param stmtNumber the target statement number in string
     * @return the statement type
     */
    StmtType getStatementType(string stmtNumber);
    /**
     * Set the statement and type into the table
     *
     * @param stmtNum the statement number in string
     * @param type the type of the statement
     * @throws runtime_error if type of stmtNumber is already set
     */
    void setStatement(string stmtNum, StmtType type);

    /**
     * Gets the number of statements stored in table
     *
     * @return set of statement numbers
     */
    int getStatementCount();

};