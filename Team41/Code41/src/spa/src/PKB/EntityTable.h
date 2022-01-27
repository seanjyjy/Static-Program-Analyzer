//
// Created by JinHao on 27/1/22.
//

#ifndef SPA_ENTITYTABLE_H
#define SPA_ENTITYTABLE_H

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


#endif //SPA_ENTITYTABLE_H
