#include "ParentTable.h"
#include <unordered_set>
#include <stdexcept>
#include <utility>

ParentTable::ParentTable() : parentRelation("Parent"), ancestorRelation("ParentT") {}

void ParentTable::setParent(const string &parentStmt, const string &childStmt) {
    // NOTE: does not check if follower is a numeric string
    if (parentStmt == childStmt) { throw domain_error("[PKB][ParentTable] Statements cannot parent self"); }

    parentRelation.addMapping(parentStmt, childStmt);
}

void ParentTable::setParentT(const string &ancestorStmt, const string &descendantStmt) {
    // NOTE: does not check if follower is a numeric string
    if (ancestorStmt == descendantStmt) { throw domain_error("[PKB][ParentTable] Statements cannot parentT self"); }

    ancestorRelation.addMapping(ancestorStmt, descendantStmt);
}

unordered_set<string> ParentTable::getAllChildrenOf(const string &parentStmt) {
    return parentRelation.getValuesFromKey(parentStmt);
}

string ParentTable::getParentOf(const string &childStatement) {
    return parentRelation.getKeyFromValue(childStatement);
}

vector<pair<string, string>> ParentTable::getParentEntries() {
    return parentRelation.getEntries();
}

bool ParentTable::isParent(const string &parentStmt, const string &childStmt) {
    return parentRelation.hasMapping(parentStmt, childStmt);
}

unordered_set<string> ParentTable::getAllDescendantsOf(const string &parentStmt) {
    return ancestorRelation.getValuesFromKey(parentStmt);
}

unordered_set<string> ParentTable::getAllAncestorsOf(const string &childStmt) {
    return ancestorRelation.getKeysFromValue(childStmt);
}

vector<pair<string, string>> ParentTable::getParentTEntries() {
    return ancestorRelation.getEntries();
}

bool ParentTable::isParentT(const string &parentStmt, const string &childStmt) {
    return ancestorRelation.hasMapping(parentStmt, childStmt);
}

unordered_set<string> ParentTable::getStmtsParentOfSomeStmt() {
    return parentRelation.getKeys();
}

unordered_set<string> ParentTable::getStmtsChildOfSomeStmt() {
    return parentRelation.getValues();
}
