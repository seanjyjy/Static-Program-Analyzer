#include "IfPatternTable.h"

#include <utility>

IfPatternTable::IfPatternTable() = default;

void IfPatternTable::setPattern(const string &stmtNum, const string &predVariable) {
    patternRelation.addMapping(stmtNum, predVariable);
}

unordered_set<string> IfPatternTable::getStmtFromVar(const string &varName) {
    return patternRelation.getKeysFromValue(varName);
}

vector<pair<string, string>> IfPatternTable::getStmtNVar() {
    return patternRelation.getEntries();
}

unordered_set<string> IfPatternTable::getStmtsUsingSomeVarInPred() {
    return patternRelation.getKeys();
}
