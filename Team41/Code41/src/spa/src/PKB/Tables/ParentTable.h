#pragma once

#include "PKB/Relations/ManyToMany.h"
#include "PKB/Relations/OneToMany.h"
#include <string>

using namespace std;

/**
 * A relational table that stores the Parent abstractions
 * - Parent relation follows One-Many relationship mapping
 * - ParentT relation follows Many-Many relationship mapping
 */
class ParentTable {
private:
    // Parent : 'Parent' '(' stmtRef ',' stmtRef ')'
    OneToMany<string, string> parentRelation;
    // ParentT : 'Parent\*' '(' stmtRef ',' stmtRef ')'
    ManyToMany<string, string> ancestorRelation;
public:
    ParentTable();

    /**
     * Set parent-child relation of 2 statements in the table
     *
     * @param parentStmt the parent statement
     * @param childStmt  the child statement
     * @throws domain_error if statement attempts to parent itself
     */
    void setParent(const string &parentStmt, const string &childStmt);

    /**
      * Set ancestor-descendant relation of 2 statements in the table
      *
      * @param parentStmt the ancestor statement
      * @param childStmt  the descendant statement
      * @throws domain_error if statement attempts to parent itself
      */
    void setParentT(const string &parentStmt, const string &childStmt);

    unordered_set<string> getAllChildrenOf(const string &parentStmt);// Gets stmts that are direct child of parent
    string getParentOf(const string &childStatement); // Gets the stmt that is parent of childStmt
    vector<pair<string, string>> getParentEntries(); // Gets list of parent-child pair
    bool isParent(const string &parentStmt, const string &childStmt); // Checks if parentStmt is parent of childStmt

    unordered_set<string> getAllDescendantsOf(const string &parentStmt); // Gets stmts that are descendants of parent
    unordered_set<string> getAllAncestorsOf(const string &childStmt); // Gets the stmts that are ancestors of child
    vector<pair<string, string>> getParentTEntries(); // Gets list of ancestor-descendant pair
    bool isParentT(const string &parentStmt, const string &childStmt); // Checks if child is a descendant of parent

    unordered_set<string> getStmtsParentOfSomeStmt(); // get set of stmts parent of some stmt
    unordered_set<string> getStmtsChildOfSomeStmt(); // get set of stmts parented by some stmt
};
