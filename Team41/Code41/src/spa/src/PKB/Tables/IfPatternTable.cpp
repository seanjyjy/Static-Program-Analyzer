#include "IfPatternTable.h"

#include <utility>

IfPatternTable::IfPatternTable() = default;

void IfPatternTable::setPattern(const string& stmtNum, const string& predVariable) {
    patternRelation.addMapping(stmtNum, predVariable);
}

unordered_set<string> IfPatternTable::getStmtFromVar(string varName) {
    return patternRelation.getKeysFromValue(move(varName));
}

vector<pair<string, string>> IfPatternTable::getStmtNVar() {
    return patternRelation.getEntries();
}

unordered_set<string> IfPatternTable::getStmtsUsingSomeVarInPred() {
    return patternRelation.getKeys();
}
