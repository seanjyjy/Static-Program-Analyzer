#pragma once

#include <string>

using namespace std;

class ClauseVariable {
private:
    string &label;
public:
    enum variable_type {
        identifier, wildcard, synonym, integer
    };
    variable_type type;
    variable_type getType();
    string &getLabel();
    bool isIdentifier();
    bool isWildCard();
    bool isSynonym();
    bool isInteger();
    ClauseVariable(variable_type type, string &label);
};
