#pragma once

#include "ManyToMany.h"
#include "OneToMany.h"
#include <string>

using namespace std;

class ParentTable {
private:
    // Parent : 'Parent' '(' stmtRef ',' stmtRef ')'
    OneToMany<string, string> parentRelation;
    // ParentT : 'Parent\*' '(' stmtRef ',' stmtRef ')'
    ManyToMany<string, string> ancestorRelation;
public:
    ParentTable();

    void setParent(string parentStmt, string childStmt);

    unordered_set<string> getAllChildrenOf(string parentStmt);
    string getParentOf(string childStatement);
    vector<pair<string, string>> getParentEntries();
    bool isParent(string parentStmt, string childStmt);

    unordered_set<string> getAllDescendantsOf(string parentStmt);
    unordered_set<string> getAllAncestorsOf(string childStmt);
    vector<pair<string, string>> getParentTEntries();
    bool isParentT(string parentStmt, string childStmt);

    unordered_set<string> getStmtsParentOfSomeStmt();
    unordered_set<string> getStmtsChildOfSomeStmt();
};
