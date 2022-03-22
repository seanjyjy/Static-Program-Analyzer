#pragma once

#include <string>
#include <vector>
#include "QueryDeclaration.h"
#include "QueryClause.h"
#include "PatternClause.h"
#include "WithClause.h"
#include "SelectTarget.h"

using namespace std;

class QueryObject {
private:
    vector<WithClause> withClauses;
    vector<SuperClause*> superClauses;
public:
    // todo: make private after all refactor
    vector<QueryDeclaration> declarations; // collection of declarations in the query
    // todo: make private after all refactor
    vector<QueryClause> clauses; // collection of clauses made during the query
    // todo: make private after all refactor
    vector<PatternClause> patternClauses; // collection of "pattern" clauses

    // todo: remove selectSynonym after all system convert
    QueryDeclaration selectSynonym; // synonym to select for the query
    // todo: selectTarget replaces selectSynonym
    // todo: make private after all refactor
    SelectTarget selectTarget = SelectTarget(SelectTarget::TUPLE);

    // todo: make private after all refactor
    bool isQueryValid; // indicates if query was valid

    vector<QueryDeclaration>& getDeclarations();
    vector<QueryClause>& getClauses();
    vector<PatternClause>& getPatternClauses();
    vector<WithClause>& getWithClauses();
    vector<SuperClause*>& getSuperClauses();

    SelectTarget& getSelectTarget();

    bool isValid();

    bool isSelectingBoolean();

    vector<Selectable> getSelectables();

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
    ~QueryObject();

    /**
     * Prints all declarations and clauses in the query object. Mainly for debugging purposes.
     */
    void print();

    /**
     * Prints all declarations in the query object. Mainly for debugging purposes.
     */
    void printDeclarations();

    /**
     * Prints all clauses in the query object. Mainly for debugging purposes.
     */
    void printClauses();
};
