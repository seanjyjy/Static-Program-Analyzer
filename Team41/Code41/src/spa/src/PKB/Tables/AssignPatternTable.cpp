#include "AssignPatternTable.h"

#include <utility>

AssignPatternTable::AssignPatternTable() = default;

void AssignPatternTable::setPattern(const string &stmtNum, const string &lhsVariable, TNode *rhsAssignAST) {
    string rhsPattern = TreeUtils::serialize(rhsAssignAST);
    auto stmtVarMapping = patternRelation.get(rhsPattern);
    stmtVarMapping.addMapping(stmtNum, lhsVariable);
    patternRelation.put(rhsPattern, stmtVarMapping);
}

unordered_set<string> AssignPatternTable::getAllStmtsFromFullPattern(TNode *patternAST) {
    string pattern = TreeUtils::serialize(patternAST);
    if (!patternRelation.hasKey(pattern)) { return {}; }
    return patternRelation.get(pattern).getKeys();
}

unordered_set<string> AssignPatternTable::getStmtFromFullPatternNVar(TNode *patternAST, string varName) {
    string pattern = TreeUtils::serialize(patternAST);
    if (!patternRelation.hasKey(pattern)) { return {}; }
    return patternRelation.get(pattern).getKeysFromValue(move(varName));
}

vector<pair<string, string>> AssignPatternTable::getStmtNVarFromFullPattern(TNode *patternAST) {
    string pattern = TreeUtils::serialize(patternAST);
    if (!patternRelation.hasKey(pattern)) { return {}; }
    return patternRelation.get(pattern).getEntries();
}

unordered_set<string> AssignPatternTable::getAllStmtsFromSubPattern(TNode *subPatternAST) {
    string subPattern = TreeUtils::serialize(subPatternAST);
    unordered_set<string> keys = patternRelation.keys();
    auto containsSubString = [&subPattern](string s) { return s.find(subPattern) != string::npos; };
    auto it = find_if(keys.begin(), keys.end(), containsSubString);
    unordered_set<string> res;

    while (it != keys.end()) {
        unordered_set<string> toMerge = patternRelation.get(*it).getKeys();
        res.insert(toMerge.begin(), toMerge.end());
        it = find_if(next(it), keys.end(), containsSubString);
    }
    return res;
}

unordered_set<string> AssignPatternTable::getStmtFromSubPatternNVar(TNode *subPatternAST, const string &varName) {
    string subPattern = TreeUtils::serialize(subPatternAST);
    unordered_set<string> keys = patternRelation.keys();
    auto containsSubString = [&subPattern](const string &s) { return s.find(subPattern) != string::npos; };
    auto it = find_if(keys.begin(), keys.end(), containsSubString);

    unordered_set<string> res;
    while (it != keys.end()) {
        unordered_set<string> toMerge = patternRelation.get(*it).getKeysFromValue(varName);
        res.insert(toMerge.begin(), toMerge.end());
        it = find_if(next(it), keys.end(), containsSubString);
    }
    return res;
}

vector<pair<string, string>> AssignPatternTable::getStmtNVarFromSubPattern(TNode *subPatternAST) {
    string subPattern = TreeUtils::serialize(subPatternAST);
    unordered_set<string> keys = patternRelation.keys();
    auto containsSubString = [&subPattern](string s) { return s.find(subPattern) != string::npos; };
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
