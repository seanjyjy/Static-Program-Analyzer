//
// Created by JinHao on 27/1/22.
//

#include "EntityTable.h"

EntityTable::EntityTable() = default;

set<string> EntityTable::getConstants() {
    return mapping.get(CONST);
}

set<string> EntityTable::getProcedures() {
    return mapping.get(PROC);
}

set<string> EntityTable::getVariables() {
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