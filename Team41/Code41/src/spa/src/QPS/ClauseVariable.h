#pragma once

#include <string>

using namespace std;

class ClauseVariable {
public:
    enum variableType {
        identifier, wildcard, synonym
    };
    variableType type;
    string &label;

    bool isIdentifier();
    bool isWildCard();
    bool isSynonym();
    ClauseVariable(variableType type, string &label);
};
