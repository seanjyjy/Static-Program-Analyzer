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
     * @param follower the follower statement
     * @param followed the followed statement
     * @throws domain_error if relation violates the table relational constraint
     */
    void setFollows(const string &follower, const string &followed);

    /**
     * Set stmt1 followsT stmt2 relation in the table
     *
     * @param follower the follower statement
     * @param followed the followed statement
     * @throws domain_error if relation violates the table relational constraint
     */
    void setFollowsT(const string &follower, const string &followed);

    string getStmtFollowedBy(string follower); // Gets the stmt that is followed by this follower
    string getStmtFollowing(string followed); // Gets the stmt that follows this followed stmt
    vector<pair<string, string>> getFollowEntries(); // Gets list of follow-follower pair
    bool isFollows(string follower, string followed); // Checks if the follower follows the followed stmt

    unordered_set<string> getStmtsFollowedTBy(string follower); // Gets set of stmts that are followedT by this follower
    unordered_set<string> getStmtsFollowingT(string followed); // Gets set of stmts that followsT this stmt
    vector<pair<string, string>> getFollowTEntries(); // Gets list of ancestor-descendant pair
    bool isFollowsT(string follower, string followed); // Checks if the follower followsT the followed stmt

    unordered_set<string> getStmtsFollowingSomeStmt(); // Get set of stmts following some stmt
    unordered_set<string> getStmtsFollowedBySomeStmt(); // Get set of stmts followed by some stmt
};
