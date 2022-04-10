#pragma once

#include <string>
#include <vector>
#include "QPS/QO/QueryDeclaration.h"
#include "QPS/QO/QueryClause.h"
#include "QPS/QO/PatternClause.h"
#include "WithClause.h"
#include "SelectTarget.h"
#include "SuperClause.h"

using namespace std;

class QueryObject {
private:
    vector<WithClause> withClauses;
    vector<SuperClause *> superClauses;
    bool useOfUndeclaredVariable;
    vector<QueryDeclaration> declarations; // collection of declarations in the query
    vector<QueryClause> clauses; // collection of clauses made during the query
    vector<PatternClause> patternClauses; // collection of "pattern" clauses
    SelectTarget selectTarget = SelectTarget(SelectTarget::TUPLE);

    bool isQueryValid; // indicates if query was valid
    size_t currPtr = 0; // pointer to the next clause to consume
public:
    /**
     * Getter for the declarations of the query.
     *
     * @return vector of QueryDeclarations.
     */
    vector<QueryDeclaration> &getDeclarations();

    /**
     * Getter for the such that clauses of the query.
     *
     * @return vector of QueryClauses.
     */
    vector<QueryClause> &getClauses();

    /**
     * Getter for the pattern clauses of the query.
     *
     * @return vector of PatternClauses.
     */
    vector<PatternClause> &getPatternClauses();

    /**
     * Getter for the with clauses of the query.
     *
     * @return vector of WithClauses.
     */
    vector<WithClause> &getWithClauses();

    /**
     * Getter for the super clauses of the query.
     *
     * @return vector of SuperClauses.
     */
    vector<SuperClause *> &getSuperClauses();

    /**
     * Getter for the Select Target of the query.
     *
     * @return SelectTarget object reference.
     */
    SelectTarget &getSelectTarget();

    /**
     * Checks if the query uses undeclared variables.
     *
     * @return boolean if query does use undeclared variables.
     */
    bool hasUseOfUndeclaredVariable() const;

    /**
     * Setter for if the query uses undeclared variables.
     *
     * @param boolean indicating if query uses undeclared variables.
     */
    void setUseOfUndeclaredVariable(bool);

    /**
     * Setter for if the query is syntactically valid.
     *
     * @param boolean indicating if query is syntactically valid.
     */
    void setValidity(bool);

    /**
     * Checks if the query is syntactically valid.
     *
     * @return boolean if query is syntactically valid.
     */
    bool isValid() const;

    /**
     * Checks if the query's select target is a boolean.
     *
     * @return boolean if select target is a boolean.
     */
    bool isSelectingBoolean();

    /**
     * Getter for the Selectables of the query's select target.
     *
     * @return vector of Selectables from the select target.
     */
    vector<Selectable> getSelectables();

    /**
     * Getter for the Selectables of the query's select target.
     * Gets the selectables as QueryDeclarations.
     *
     * @return vector of QueryDeclarations from the select target.
     */
    vector<QueryDeclaration> getSelectablesAsQDs();

    /**
     * Constructor for the QueryObject class.
     *
     * @param declarations vector storing declared entities.
     * @param clauses vector storing query clauses.
     * @param patternClauses vector storing "pattern" clauses.
     * @param selectSynonym synonym to select for the query.
     * @param isQueryValid query's validity.
     */
    QueryObject(vector<QueryDeclaration> declarations, vector<QueryClause> clauses,
                vector<PatternClause> patternClauses, vector<WithClause> withClauses,
                vector<SuperClause *> superClauses, SelectTarget selectTarget, bool isQueryValid);

    virtual ~QueryObject() = default;

    /**
     * Returns the next unconsumed clause. Does not modify the actual underlying clause collection.
     */
    virtual SuperClause *popClause();

    /**
     * Returns the size of the current clause group. By default, there is one clause group (of all parsed clauses).
     */
    virtual size_t currGroupSize();

    /**
     * Returns true if the current group can be simplified, false otherwise.
     */
    virtual bool currGroupCanSimplify();

    /**
     * Returns true if after one more clause is popped, the current group will be empty, false otherwise.
     */
    virtual bool isLastOfGroup();

    /**
     * Returns true if there are no more clauses to pop from all groups, false otherwise.
     */
    virtual bool empty();

    /**
     * Prints all declarations and clauses in the query object. Mainly for debugging purposes.
     */
    virtual void print();

    /**
     * Prints all declarations in the query object. Mainly for debugging purposes.
     */
    void printDeclarations();

    /**
     * Prints all clauses in the query object. Mainly for debugging purposes.
     */
    void printClauses();

    /**
     * Clean up all allocated heap memory
     */
    void cleanUp();
};
