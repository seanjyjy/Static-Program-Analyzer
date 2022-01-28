//
// Created by JinHao on 27/1/22.
//
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
    set<string> getConstants();
    set<string> getVariables();
    set<string> getProcedures();
};

