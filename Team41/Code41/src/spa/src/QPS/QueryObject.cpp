#include "QueryObject.h"

#include <utility>

QueryObject::QueryObject(vector<QueryDeclaration> declarations,
                         vector<QueryClause> clauses,
                         vector<PatternClause> patternClauses,
                         vector<WithClause> withClauses,
                         QueryDeclaration selectSynonym,
                         SelectTarget selectTarget,
                         bool isQueryValid) :
    declarations(std::move(declarations)),
    clauses(std::move(clauses)),
    patternClauses(std::move(patternClauses)),
    withClauses(std::move(withClauses)),
    selectSynonym(std::move(selectSynonym)),
    selectTarget(std::move(selectTarget)),
    isQueryValid(isQueryValid) {
}

QueryObject::QueryObject(vector<QueryDeclaration> declarations,
                         vector<QueryClause> clauses,
                         vector<PatternClause> patternClauses,
                         QueryDeclaration selectSynonym,
                         bool isQueryValid) :
        declarations(std::move(declarations)),
        clauses(std::move(clauses)),
        patternClauses(std::move(patternClauses)),
        selectSynonym(std::move(selectSynonym)),
        isQueryValid(isQueryValid) {
}

vector<QueryDeclaration>& QueryObject::getDeclarations() {
    return declarations;
}

vector<QueryClause>& QueryObject::getClauses() {
    return clauses;
}

vector<PatternClause>& QueryObject::getPatternClauses() {
    return patternClauses;
}

vector<WithClause>& QueryObject::getWithClauses() {
    return withClauses;
}

SelectTarget QueryObject::getSelectTarget() {
    return selectTarget;
}

bool QueryObject::isValid() {
    return isQueryValid;
}

bool QueryObject::isSelectingBoolean() {
    return selectTarget.isBoolean();
}

vector<Selectable> QueryObject::getSelectables() {
    return selectTarget.tuple;
}

QueryObject::~QueryObject() {
    for (PatternClause pList: patternClauses) {
        for (PatternVariable p : pList.getRHS()){
            p.cleanAST();
        }
    }
}

QueryDeclaration::design_entity_type QueryDeclaration::getType() {
    return type;
}

string QueryDeclaration::getSynonym() {
    return synonym;
}

void QueryObject::print() {
    printDeclarations();
    printClauses();
}

void QueryObject::printDeclarations() {
    cout << "*** DECLARATIONS ***" << endl;
    for (QueryDeclaration &qd: declarations) {
        qd.print();
    }
}

void QueryObject::printClauses() {
    cout << "*** CLAUSES ***" << endl;
    for (QueryClause &qc: clauses) {
        qc.print();
    }
}
