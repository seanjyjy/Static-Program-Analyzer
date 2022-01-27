//
// Created by JinHao on 26/1/22.
//
#pragma once

#ifndef SPA_STMTTABLE_H
#define SPA_STMTTABLE_H

#include "OneToMany.h"

#include "StmtType.h"

using namespace std;
class StmtTable {
private:
    OneToMany<StmtType, string> mapping;
public:
    StmtTable();

    /**
     * Gets set of statement that belongs to the specified type
     *
     * @param type the target SIMPLE statement type
     * @return the set of numeric string representing the stmt numbers
     */
    set<string> getAllStmtsByType(StmtType type);

    /**
     * Gets all the statement type of the specified statement number
     *
     * @param stmtNumber the target statement number in string
     * @return the statement type
     */
    StmtType getStmtType(string stmtNumber);

    /**
     * Checks if the statement number match the specified type
     *
     * @param stmtNumber the statement number in string
     * @param type the queried statement type
     * @return true if matches, false otherwise
     */
    bool isStmtType(string stmtNumber, StmtType type);

    /**
     * Set the statement and type into the table
     *
     * @param stmtNum the statement number in string
     * @param type the type of the statement
     * @throws error if type of stmtNumber is already set
     */
    void setStmt(string stmtNum, StmtType type);

    /**
     * Gets the number of statements stored in table
     *
     * @return set of statement numbers
     */
    int getStmtCount();

    /**
     * Gets all statements added to the table
     *
     * @return set of statement numbers
     */
    set<string> getAllStmts();

    /**
     * Gets all statement types added to the table
     *
     * @return set of statement types
     */
    set<StmtType> getAllTypes();
};


#endif //SPA_STMTTABLE_H
