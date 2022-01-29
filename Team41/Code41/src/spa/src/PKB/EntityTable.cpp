#include "EntityTable.h"

EntityTable::EntityTable() = default;

unordered_set<string> EntityTable::getConstants() {
    return mapping.get(CONST);
}

unordered_set<string> EntityTable::getProcedures() {
    return mapping.get(PROC);
}

unordered_set<string> EntityTable::getVariables() {
    return mapping.get(VAR);
}

void EntityTable::addConstant(string numStr) {
    mapping.add(CONST, numStr);
}

void EntityTable::addProcedure(string procName) {
    mapping.add(PROC, procName);
}

void EntityTable::addVariable(string varName) {
    mapping.add(VAR, varName);
}