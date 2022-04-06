#pragma once

#include <string>
#include <iostream>
#include <stdexcept>

#include "QPS/Entities/EntitiesType/Entities.h"
#include "QPS/Entities/EntitiesType/StmtEntities.h"
#include "QPS/Entities/EntitiesType/ReadEntities.h"
#include "QPS/Entities/EntitiesType/PrintEntities.h"
#include "QPS/Entities/EntitiesType/CallEntities.h"
#include "QPS/Entities/EntitiesType/WhileEntities.h"
#include "QPS/Entities/EntitiesType/IfEntities.h"
#include "QPS/Entities/EntitiesType/AssignEntities.h"
#include "QPS/Entities/EntitiesType/VariableEntities.h"
#include "QPS/Entities/EntitiesType/ConstantEntities.h"
#include "QPS/Entities/EntitiesType/ProcedureEntities.h"

using namespace std;

class QueryDeclaration {
private:
    // declaration type of the declaration instance
    Entities *type;

    // accompanying synonym of a declaration
    string synonym;
public:
    /**
     * Getter for the type of the declaration.
     *
     * @return Entities* representing the type.
     */
    Entities *getType() const;

    /**
     * Getter for the synonym of the declaration.
     *
     * @return string representing the synonym.
     */
    string getSynonym() const;

    /**
     * Constructor for the QueryDeclaration class.
     *
     * @param type of the new declaration.
     * @param synonym string accompanying the new declaration.
     */
    QueryDeclaration(Entities *type, string &synonym);
    QueryDeclaration();

    /**
     * Converts a string representation of the declaration type
     * to the system representation using the enum.
     *
     * @param s string representing the declaration type.
     * @return design_entity_type corresponding to the declaration.
     */
    static Entities* stringToType(string &s);

    /**
     * Converts the enum design entity type into a human-readable string representation.
     *
     * @param d a valid design entity type as specified by the enum.
     * @return a human-readable string representation.
     * @throws runtime_error if the given design entity type has no string mapping.
     */
    static string typeToString(Entities *d);

    /**
     * Returns a human-readable string representation of the query declaration.
     */
    string toString();

    /**
     * Prints the query declaration's type and synonym in a human-readable form.
     */
    void print();

    /**
     * Equivalence comparator for the QueryDeclaration class.
     *
     * @return boolean indicating if equivalent.
     */
    bool equals(QueryDeclaration) const;

    void cleanUp();
};
