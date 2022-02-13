#pragma once

#include <string>
#include "QueryDeclaration.h"

using namespace std;

class ClauseVariable {
private:
    string label;
    QueryDeclaration::design_entity_type designEntityType;
public:
    enum variable_type {
        identifier, wildcard, synonym, integer
    };
    variable_type type;
    variable_type getType();
    string getLabel();
    QueryDeclaration::design_entity_type getDesignEntityType();
    bool isIdentifier();
    bool isWildCard();
    bool isSynonym();
    bool isInteger();
    ClauseVariable(variable_type type, string label, QueryDeclaration::design_entity_type designEntityType);
};
