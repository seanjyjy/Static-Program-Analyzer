#include "PatternTable.h"

PatternTable::PatternTable() = default;

void PatternTable::setPattern(string stmtNum, string lhsVariable, TNode *rhsAssignAST) {
    string rhsPattern = TreeUtils::serialize(rhsAssignAST);
    auto stmtVarMapping = patternRelation.get(rhsPattern);
    stmtVarMapping.addMapping(stmtNum, lhsVariable);
    patternRelation.put(rhsPattern, stmtVarMapping);
}

unordered_set<string> PatternTable::getAllStmtsFromFullPattern(TNode *patternAST) {
    string pattern = TreeUtils::serialize(patternAST);
    if (!patternRelation.hasKey(pattern)) { return unordered_set<string>(); }
    return patternRelation.get(pattern).getKeys();
}

unordered_set<string> PatternTable::getStmtFromFullPatternNVar(TNode *patternAST, string varName) {
    string pattern = TreeUtils::serialize(patternAST);
    if (!patternRelation.hasKey(pattern)) { return unordered_set<string>(); }
    return patternRelation.get(pattern).getKeysFromValue(varName);
}

vector<pair<string, string>> PatternTable::getStmtNVarFromFullPattern(TNode *patternAST) {
    string pattern = TreeUtils::serialize(patternAST);
    if (!patternRelation.hasKey(pattern)) { return vector<pair<string, string>>(); }
    return patternRelation.get(pattern).getEntries();
}

unordered_set<string> PatternTable::getAllStmtsFromSubPattern(TNode *subPatternAST) {
    string subPattern = TreeUtils::serialize(subPatternAST);
    unordered_set<string> keys = patternRelation.keys();
    auto containsSubString = [&subPattern](string s) { return s.find(subPattern) != std::string::npos; };
    auto it = find_if(keys.begin(), keys.end(), containsSubString);
    unordered_set<string> res;

    while (it != keys.end()) {
        res.merge(patternRelation.get(*it).getKeys());
        it = find_if(next(it), keys.end(), containsSubString);
    }
    return res;
}

unordered_set<string> PatternTable::getStmtFromSubPatternNVar(TNode *subPatternAST, string varName) {
    string subPattern = TreeUtils::serialize(subPatternAST);
    unordered_set<string> keys = patternRelation.keys();
    auto containsSubString = [&subPattern](string s) { return s.find(subPattern) != std::string::npos; };
    auto it = find_if(keys.begin(), keys.end(), containsSubString);

    unordered_set<string> res;
    while (it != keys.end()) {
        res.merge(patternRelation.get(*it).getKeysFromValue(varName));
        it = find_if(next(it), keys.end(), containsSubString);
    }
    return res;
}

vector<pair<string, string>> PatternTable::getStmtNVarFromSubPattern(TNode *subPatternAST) {
    string subPattern = TreeUtils::serialize(subPatternAST);
    unordered_set<string> keys = patternRelation.keys();
    auto containsSubString = [&subPattern](string s) { return s.find(subPattern) != std::string::npos; };
    auto it = find_if(keys.begin(), keys.end(), containsSubString);

    // ensured to be unique
    vector<pair<string, string>> res;
    while (it != keys.end()) {
        vector<pair<string, string>> toMerge = patternRelation.get(*it).getEntries();
        res.insert(res.end(), toMerge.begin(), toMerge.end());
        it = find_if(next(it), keys.end(), containsSubString);
    }
    return res;
}
