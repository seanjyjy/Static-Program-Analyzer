#include "FollowsTable.h"
#include <unordered_set>
#include <stdexcept>
#include <utility>

FollowsTable::FollowsTable() : followsRelation("Follows"), followsTRelation("FollowsT") {}

void FollowsTable::setFollows(const string &before, const string &after) {
    // NOTE: does not check if follower is a numeric string
    if (before == after) { throw domain_error("[PKB][FollowsTable] Statements cannot follow self"); }

    followsRelation.addMapping(before, after);
}

void FollowsTable::setFollowsT(const string &before, const string &after) {
    // NOTE: does not check if follower is a numeric string
    if (before == after) { throw domain_error("[PKB][FollowsTable] Statements cannot followT self"); }

    followsTRelation.addMapping(before, after);
}

string FollowsTable::getStmtDirectlyAfter(const string &stmt) {
    return followsRelation.getValFromKey(stmt);
}

string FollowsTable::getStmtDirectlyBefore(const string &stmt) {
    return followsRelation.getKeyFromValue(stmt);
}

vector<pair<string, string>> FollowsTable::getFollowEntries() {
    return followsRelation.getEntries();
}

bool FollowsTable::isFollows(const string &follower, const string &followed) {
    return followsRelation.hasMapping(follower, followed);
}

unordered_set<string> FollowsTable::getStmtsAfter(const string &stmt) {
    return followsTRelation.getValuesFromKey(stmt);
}

unordered_set<string> FollowsTable::getStmtsBefore(const string &stmt) {
    return followsTRelation.getKeysFromValue(stmt);
}

vector<pair<string, string>> FollowsTable::getFollowTEntries() {
    return followsTRelation.getEntries();
}

bool FollowsTable::isFollowsT(const string &before, const string &after) {
    return followsTRelation.hasMapping(before, after);
}

unordered_set<string> FollowsTable::getStmtsBeforeSomeStmt() {
    return followsRelation.getKeys();
}

unordered_set<string> FollowsTable::getStmtsAfterSomeStmt() {
    return followsRelation.getValues();
}
