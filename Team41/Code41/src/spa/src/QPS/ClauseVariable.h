#pragma once

#include <string>

using namespace std;

class ClauseVariable {
private:
    enum variable_type {
        identifier, wildcard, synonym, integer
    };
    string &label;
    variable_type type;
public:
    variable_type getType();
    string &getLabel();
    bool isIdentifier();
    bool isWildCard();
    bool isSynonym();
    bool isInteger();
    ClauseVariable(variable_type type, string &label);
};
