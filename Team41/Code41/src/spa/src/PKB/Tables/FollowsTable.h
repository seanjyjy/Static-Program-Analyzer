#pragma once

#include "PKB/Relations/ManyToMany.h"
#include "PKB/Relations/OneToOne.h"
#include <string>
#include <stdexcept>

using namespace std;

/**
 * A relational table that stores the Follows abstractions
 * - Follows relation follows One-One relationship mapping
 * - FollowsT relation follows Many-Many relationship mapping
 */
class FollowsTable {
private:
    // Follows : 'Follows' '(' stmtRef ',' stmtRef ')'
    OneToOne<string, string> followsRelation;
    // FollowsT : 'Follows\*' '(' stmtRef ',' stmtRef ')'
    ManyToMany<string, string> followsTRelation;
public:
    FollowsTable();

    /**
     * Set follower follows followed relation in the table
     *
     * @param before the follower statement
     * @param after the followed statement
     * @throws domain_error if relation violates the table relational constraint
     */
    void setFollows(const string &before, const string &after);

    /**
     * Set stmt1 followsT stmt2 relation in the table
     *
     * @param before the follower statement
     * @param after the followed statement
     * @throws domain_error if relation violates the table relational constraint
     */
    void setFollowsT(const string &before, const string &after);

    string getStmtDirectlyAfter(string stmt); // Gets the stmt that is directly after by this stmt
    string getStmtDirectlyBefore(string stmt); // Gets the stmt that is directly before this stmt
    vector<pair<string, string>> getFollowEntries(); // Gets list of before-after pair
    bool isFollows(string stmt1, string stmt2); // Checks if stmt1 comes directly before stmt2

    unordered_set<string> getStmtsAfter(string stmt); // Gets set of stmts that are after this stmt
    unordered_set<string> getStmtsBefore(string stmt); // Gets set of stmts that are before this stmt
    vector<pair<string, string>> getFollowTEntries(); // Gets list of beforeT-afterT pair
    bool isFollowsT(string stmt1, string stmt2); // Checks if stmt1 comes before stmt2

    unordered_set<string> getStmtsAfterSomeStmt(); // Get set of stmts following some stmt
    unordered_set<string> getStmtsBeforeSomeStmt(); // Get set of stmts followed by some stmt
};
