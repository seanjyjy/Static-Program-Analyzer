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

string FollowsTable::getStmtDirectlyAfter(string stmt) {
    return followsRelation.getValFromKey(move(stmt));
}

string FollowsTable::getStmtDirectlyBefore(string stmt) {
    return followsRelation.getKeyFromValue(move(stmt));
}

vector<pair<string, string>> FollowsTable::getFollowEntries() {
    return followsRelation.getEntries();
}

bool FollowsTable::isFollows(string follower, string followed) {
    return followsRelation.hasMapping(move(follower), move(followed));
}

unordered_set<string> FollowsTable::getStmtsAfter(string stmt) {
    return followsTRelation.getValuesFromKey(move(stmt));
}

unordered_set<string> FollowsTable::getStmtsBefore(string stmt) {
    return followsTRelation.getKeysFromValue(move(stmt));
}

vector<pair<string, string>> FollowsTable::getFollowTEntries() {
    return followsTRelation.getEntries();
}

bool FollowsTable::isFollowsT(string before, string after) {
    return followsTRelation.hasMapping(move(before), move(after));
}

unordered_set<string> FollowsTable::getStmtsBeforeSomeStmt() {
    return followsRelation.getKeys();
}

unordered_set<string> FollowsTable::getStmtsAfterSomeStmt() {
    return followsRelation.getValues();
}
