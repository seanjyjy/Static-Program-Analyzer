#include "EntityTable.h"
#include <stdexcept>

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
    if (mapping.hasKeyValue(PROC, procName)) {
        throw runtime_error("[PKB][EntityTable] Procedure name already declared");
    }
    return mapping.add(PROC, procName);
}

void EntityTable::addVariable(string varName) {
    mapping.add(VAR, varName);
}

bool EntityTable::isConstant(string constVal) {
    return mapping.hasKeyValue(CONST, constVal);
}

bool EntityTable::isProcedure(string procName) {
    return mapping.hasKeyValue(PROC, procName);
}

bool EntityTable::isVariable(string varName) {
    return mapping.hasKeyValue(VAR, varName);
}