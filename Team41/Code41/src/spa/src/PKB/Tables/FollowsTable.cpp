#include "FollowsTable.h"
#include <unordered_set>
#include <stdexcept>
#include <utility>

FollowsTable::FollowsTable() : followsRelation("Follows"), followsTRelation("FollowsT") {}

void FollowsTable::setFollows(const string &follower, const string &followed) {
    // NOTE: does not check if follower is a numeric string
    if (follower == followed) { throw domain_error("[PKB][FollowsTable] Statements cannot follow self"); }

    followsRelation.addMapping(follower, followed);
}

void FollowsTable::setFollowsT(const string &follower, const string &followed) {
    // NOTE: does not check if follower is a numeric string
    if (follower == followed) { throw domain_error("[PKB][FollowsTable] Statements cannot followT self"); }

    followsTRelation.addMapping(follower, followed);
}

string FollowsTable::getStmtFollowedBy(string follower) {
    return followsRelation.getValFromKey(move(follower));
}

string FollowsTable::getStmtFollowing(string followed) {
    return followsRelation.getKeyFromValue(move(followed));
}

vector<pair<string, string>> FollowsTable::getFollowEntries() {
    return followsRelation.getEntries();
}

bool FollowsTable::isFollows(string follower, string followed) {
    return followsRelation.hasMapping(move(follower), move(followed));
}

unordered_set<string> FollowsTable::getStmtsFollowedTBy(string follower) {
    return followsTRelation.getValuesFromKey(move(follower));
}

unordered_set<string> FollowsTable::getStmtsFollowingT(string followed) {
    return followsTRelation.getKeysFromValue(move(followed));
}

vector<pair<string, string>> FollowsTable::getFollowTEntries() {
    return followsTRelation.getEntries();
}

bool FollowsTable::isFollowsT(string follower, string followed) {
    return followsTRelation.hasMapping(move(follower), move(followed));
}

unordered_set<string> FollowsTable::getStmtsFollowedBySomeStmt() {
    return followsRelation.getValues();
}

unordered_set<string> FollowsTable::getStmtsFollowingSomeStmt() {
    return followsRelation.getKeys();
}
