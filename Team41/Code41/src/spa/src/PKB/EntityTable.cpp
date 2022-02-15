#include "EntityTable.h"
#include <stdexcept>
#include <utility>

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

void EntityTable::addConstant(const string& numStr) {
    mapping.add(CONST, numStr);
}

void EntityTable::addProcedure(const string& procName) {
    if (mapping.hasKeyValue(PROC, procName)) {
        throw runtime_error("[PKB][EntityTable] Procedure name already declared");
    }
    return mapping.add(PROC, procName);
}

void EntityTable::addVariable(const string& varName) {
    mapping.add(VAR, varName);
}

bool EntityTable::isConstant(string constVal) {
    return mapping.hasKeyValue(CONST, move(constVal));
}

bool EntityTable::isProcedure(string procName) {
    return mapping.hasKeyValue(PROC, move(procName));
}

bool EntityTable::isVariable(string varName) {
    return mapping.hasKeyValue(VAR, move(varName));
}