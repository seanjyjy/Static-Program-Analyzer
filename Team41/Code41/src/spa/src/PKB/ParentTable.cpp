#include "ParentTable.h"
#include <unordered_set>
#include <stdexcept>

ParentTable::ParentTable() : parentRelation("Parent"), ancestorRelation("ParentT") {}

void ParentTable::setParent(string parentStmt, string childStmt) {
    // NOTE: does not check if follower is a numeric string
    if (parentStmt == childStmt) { throw domain_error("[PKB][ParentTable] Statements cannot parent self"); }
    if (isParent(parentStmt, childStmt)) { return; }

    unordered_set<string> allAncestors = getAllAncestorsOf(parentStmt);
    allAncestors.insert(parentStmt);
    unordered_set<string> allDescendants = getAllDescendantsOf(childStmt);
    allDescendants.insert(childStmt);

    if (allDescendants.find(parentStmt) != allDescendants.end()) {
        throw domain_error("[PKB][ParentTable] Cyclic dependency detected in parenting graph");
    }

    parentRelation.addMapping(parentStmt, childStmt);

    for (string parent: allAncestors) {
        for (string child: allDescendants) {
            ancestorRelation.addMapping(parent, child);
        }
    }
}

unordered_set<string> ParentTable::getAllChildrenOf(string parentStmt) {
    return parentRelation.getValuesFromKey(parentStmt);
}

string ParentTable::getParentOf(string childStatement) {
    return parentRelation.getKeyFromValue(childStatement);
}

vector<pair<string, string>> ParentTable::getParentEntries() {
    return parentRelation.getEntries();
}

bool ParentTable::isParent(string parentStmt, string childStmt) {
    return parentRelation.hasMapping(parentStmt, childStmt);
}

unordered_set<string> ParentTable::getAllDescendantsOf(string parentStmt) {
    return ancestorRelation.getValuesFromKey(parentStmt);
}

unordered_set<string> ParentTable::getAllAncestorsOf(string childStmt) {
    return ancestorRelation.getKeysFromValue(childStmt);
}

vector<pair<string, string>> ParentTable::getParentTEntries() {
    return ancestorRelation.getEntries();
}

bool ParentTable::isParentT(string parentStmt, string childStmt) {
    return ancestorRelation.hasMapping(parentStmt, childStmt);
}

unordered_set<string> ParentTable::getStmtsParentOfSomeStmt() {
    return parentRelation.getKeys();
}

unordered_set<string> ParentTable::getStmtsChildOfSomeStmt() {
    return parentRelation.getValues();
}
