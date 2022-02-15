#pragma once

#include "ManyToMany.h"
#include "OneToOne.h"
#include <string>
#include <stdexcept>

using namespace std;

class FollowsTable {
private:
    // Follows : 'Follows' '(' stmtRef ',' stmtRef ')'
    OneToOne<string, string> followsRelation;
    // FollowsT : 'Follows\*' '(' stmtRef ',' stmtRef ')'
    ManyToMany<string, string> followsTRelation;
public:
    FollowsTable();

    void setFollows(const string& follower, const string& followed);
    void setFollowsT(const string& follower, const string& followed);

    string getStmtFollowedBy(string follower);
    string getStmtFollowing(string followed);
    vector<pair<string, string>> getFollowEntries();
    bool isFollows(string follower, string followed);

    unordered_set<string> getStmtsFollowedTBy(string follower);
    unordered_set<string> getStmtsFollowingT(string followed);
    vector<pair<string, string>> getFollowTEntries();
    bool isFollowsT(string follower, string followed);

    unordered_set<string> getStmtsFollowingSomeStmt();
    unordered_set<string> getStmtsFollowedBySomeStmt();
};
