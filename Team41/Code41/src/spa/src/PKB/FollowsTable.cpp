#include "FollowsTable.h"
#include <unordered_set>
#include <stdexcept>

FollowsTable::FollowsTable() : followsRelation("Follows"), followsTRelation("FollowsT") {}

void FollowsTable::setFollows(string follower, string followed) {
    // NOTE: does not check if follower is a numeric string
    if (follower == followed) { throw domain_error("[PKB][FollowsTable] Statements cannot follow self"); }
    if (isFollows(follower, followed)) { return; }

    followsRelation.addMapping(follower, followed);
}

void FollowsTable::setFollowsT(string follower, string followed) {
    // NOTE: does not check if follower is a numeric string
    if (follower == followed) { throw domain_error("[PKB][FollowsTable] Statements cannot followT self"); }
    if (isFollowsT(follower, followed)) { return; }

    followsTRelation.addMapping(follower, followed);
}

string FollowsTable::getStmtFollowedBy(string follower) {
    return followsRelation.getValFromKey(follower);
}

string FollowsTable::getStmtFollowing(string followed) {
    return followsRelation.getKeyFromValue(followed);
}

vector<pair<string, string>> FollowsTable::getFollowEntries() {
    return followsRelation.getEntries();
}

bool FollowsTable::isFollows(string follower, string followed) {
    return followsRelation.hasMapping(follower, followed);
}

unordered_set<string> FollowsTable::getStmtsFollowedTBy(string follower) {
    return followsTRelation.getValuesFromKey(follower);
}

unordered_set<string> FollowsTable::getStmtsFollowingT(string followed) {
    return followsTRelation.getKeysFromValue(followed);
}

vector<pair<string, string>> FollowsTable::getFollowTEntries() {
    return followsTRelation.getEntries();
}

bool FollowsTable::isFollowsT(string follower, string followed) {
    return followsTRelation.hasMapping(follower, followed);
}

unordered_set<string> FollowsTable::getStmtsFollowedBySomeStmt() {
    return followsRelation.getValues();
}

unordered_set<string> FollowsTable::getStmtsFollowingSomeStmt() {
    return followsRelation.getKeys();
}
