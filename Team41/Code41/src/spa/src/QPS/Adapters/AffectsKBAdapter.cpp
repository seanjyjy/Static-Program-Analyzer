#include "AffectsKBAdapter.h"

AffectsKBAdapter::AffectsKBAdapter(PKBClient *pkb): pkb(pkb) {
    this->cache = new Cache();
}

AffectsKBAdapter::~AffectsKBAdapter() {
    delete cache;
}
// CFG
// Affects(1, s)
// stmt1 vs stmt x (bfs pop)
// Affects(s1, s2) Affects(1, s)

/*
Uses(a1, v)
Not Modifies(s, v) where s in in the path → s is a stmt number
Where s is not if/while/print
Modifies(a2, v)
 */

// Affects Graph -> O(n^3)
// AffectsT(1, s)

// AFFECTS NO CACHE -> ask kendrick
// AFFECTS GRAPH -> ASK MINSUK
// normal affects will always do BFS/DFS

// affects T will be building the cache
bool AffectsKBAdapter::isAffects(const string &stmt1, const string &stmt2) {
    return false;
}

unordered_set<string> AffectsKBAdapter::getDirectAffectsBy(const string &stmt) {
    return unordered_set<string>();
}

unordered_set<string> AffectsKBAdapter::getDirectAffecting(const string &stmt) {
    return unordered_set<string>();
}

unordered_set<string> AffectsKBAdapter::getAllStmtAffectingOther() {
    return unordered_set<string>();
}

unordered_set<string> AffectsKBAdapter::getAllStmtAffectedByOther() {
    return unordered_set<string>();
}

vector<pair<string, string>> AffectsKBAdapter::getDirectAffectsAll() {
    return vector<pair<string, string>>();
}
