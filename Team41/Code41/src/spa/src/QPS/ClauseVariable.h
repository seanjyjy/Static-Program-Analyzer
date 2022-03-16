#pragma once

#include <string>
#include "QueryDeclaration.h"

using namespace std;

class ClauseVariable {
private:
    // name of the clause variable
    string label;

    // declaration type of the clause variable (must be declared)
    QueryDeclaration::design_entity_type designEntityType;
public:
    // enum representing possible variable types in PQL
    enum variable_type {
        identifier, wildcard, synonym, integer
    };

    // variable type of the variable instance
    variable_type type;

    /**
     * Getter for the variable type.
     *
     * @return variable_type representing the type.
     */
    [[nodiscard]] variable_type getType() const;

    /**
     * Getter for the label.
     *
     * @return string representing the label.
     */
    [[nodiscard]] string getLabel() const;

    /**
     * Getter for the declaration type.
     *
     * @return QueryDeclaration::design_entity_type representing the declaration type.
     */
    QueryDeclaration::design_entity_type getDesignEntityType() const;

    /**
     * Checks if the variable is an identifier.
     *
     * @return boolean indicating if identifier.
     */
    bool isIdentifier();

    /**
     * Checks if the variable is a wildcard.
     *
     * @return boolean indicating if wildcard.
     */
    bool isWildCard();

    /**
     * Checks if the variable is an synonym.
     *
     * @return boolean indicating if synonym.
     */
    bool isSynonym();

    /**
     * Checks if the variable is an integer.
     *
     * @return boolean indicating if integer.
     */
    bool isInteger();

    /**
     * Constructor for the ClauseVariable class.
     *
     * @param type of the new variable.
     * @param label for the name of the new variable.
     * @param designEntityType for the declaration type of the new variable.
     */
    ClauseVariable(variable_type type, string label, QueryDeclaration::design_entity_type designEntityType);
};
