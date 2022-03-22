#pragma once

#include "PKB/PKBClient.h"
#include "QPS/Cache/Cache.h"

class AffectsKBAdapter {
private:
    PKBClient* pkb;
    Cache* cache;
public:
    explicit AffectsKBAdapter(PKBClient* pkb);
    ~AffectsKBAdapter();

    bool isAffects(const string& stmt1, const string& stmt2);
    unordered_set<string> getDirectAffectsBy(const string& stmt);
    unordered_set<string> getDirectAffecting(const string& stmt);
    unordered_set<string> getAllStmtAffectingOther();
    unordered_set<string> getAllStmtAffectedByOther();
    vector<pair<string, string>> getDirectAffectsAll();
};