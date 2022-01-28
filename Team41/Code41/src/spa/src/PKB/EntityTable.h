#pragma once

#include <string>

#include "MultiMap.h"

using namespace std;

enum EntityType {
    UNKNOWN_ENTITY,
    CONST,
    VAR,
    PROC
};

/**
 * Store list of entities used in the SIMPLE program
 */
class EntityTable {
private:
    MultiMap<EntityType, string> mapping;
public:
    EntityTable();
    void addConstant(string numStr);
    void addVariable(string varName);
    void addProcedure(string procName);
    unordered_set<string> getConstants();
    unordered_set<string> getVariables();
    unordered_set<string> getProcedures();
};

