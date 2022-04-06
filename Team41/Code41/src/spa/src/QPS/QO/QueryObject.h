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
    vector<SuperClause*> superClauses;
    bool useOfUndeclaredVariable;
    vector<QueryDeclaration> declarations; // collection of declarations in the query
    vector<QueryClause> clauses; // collection of clauses made during the query
    vector<PatternClause> patternClauses; // collection of "pattern" clauses
    QueryDeclaration selectSynonym; // synonym to select for the query
    SelectTarget selectTarget = SelectTarget(SelectTarget::TUPLE);

    bool isQueryValid; // indicates if query was valid
    size_t currPtr = 0; // pointer to the next clause to consume
public:


    vector<QueryDeclaration>& getDeclarations();
    vector<QueryClause>& getClauses();
    vector<PatternClause>& getPatternClauses();
    vector<WithClause>& getWithClauses();
    vector<SuperClause*>& getSuperClauses();
    SelectTarget& getSelectTarget();

    bool hasUseOfUndeclaredVariable() const;
    void setUseOfUndeclaredVariable(bool);

    void setValidity(bool);

    bool isValid() const;

    bool isSelectingBoolean();

    vector<Selectable> getSelectables();

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
    QueryObject(vector<QueryDeclaration> declarations, vector<QueryClause> clauses, vector<PatternClause> patternClauses, vector<WithClause> withClauses, vector<SuperClause*> superClauses, QueryDeclaration selectSynonym, SelectTarget selectTarget, bool isQueryValid);

    // old
    QueryObject(vector<QueryDeclaration> declarations, vector<QueryClause> clauses, vector<PatternClause> patternClauses, QueryDeclaration selectSynonym, bool isQueryValid);
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
