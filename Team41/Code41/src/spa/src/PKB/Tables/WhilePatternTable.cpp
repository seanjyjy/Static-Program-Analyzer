#include "WhilePatternTable.h"

#include <utility>

WhilePatternTable::WhilePatternTable() = default;

void WhilePatternTable::setPattern(const string &stmtNum, const string &predVariable) {
    patternRelation.addMapping(stmtNum, predVariable);
}

unordered_set<string> WhilePatternTable::getStmtFromVar(string varName) {
    return patternRelation.getKeysFromValue(varName);
}

vector<pair<string, string>> WhilePatternTable::getStmtNVar() {
    return patternRelation.getEntries();
}

unordered_set<string> WhilePatternTable::getStmtsUsingSomeVarInPred() {
    return patternRelation.getKeys();
}
