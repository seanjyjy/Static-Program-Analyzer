#include "QueryObject.h"

#include <utility>

QueryObject::QueryObject(vector<QueryDeclaration> declarations,
                         vector<QueryClause> clauses,
                         vector<PatternClause> patternClauses,
                         vector<WithClause> withClauses,
                         vector<SuperClause *> superClauses,
                         SelectTarget selectTarget,
                         bool isQueryValid) :
        withClauses(std::move(withClauses)),
        superClauses(std::move(superClauses)),
        useOfUndeclaredVariable(false),
        declarations(std::move(declarations)),
        clauses(std::move(clauses)),
        patternClauses(std::move(patternClauses)),
        selectTarget(std::move(selectTarget)),
        isQueryValid(isQueryValid) {
}
//
//QueryObject::QueryObject(vector<QueryDeclaration> declarations,
//                         vector<QueryClause> clauses,
//                         vector<PatternClause> patternClauses,
//                         bool isQueryValid) :
//        useOfUndeclaredVariable(false),
//        declarations(std::move(declarations)),
//        clauses(std::move(clauses)),
//        patternClauses(std::move(patternClauses)),
//        isQueryValid(isQueryValid) {
//}

vector<QueryDeclaration> &QueryObject::getDeclarations() {
    return declarations;
}

vector<QueryClause> &QueryObject::getClauses() {
    return clauses;
}

vector<PatternClause> &QueryObject::getPatternClauses() {
    return patternClauses;
}

vector<WithClause> &QueryObject::getWithClauses() {
    return withClauses;
}

vector<SuperClause *> &QueryObject::getSuperClauses() {
    return superClauses;
}

SelectTarget &QueryObject::getSelectTarget() {
    return selectTarget;
}

bool QueryObject::isValid() const {
    return isQueryValid;
}

void QueryObject::setValidity(bool v) {
    isQueryValid = v;
}

bool QueryObject::isSelectingBoolean() {
    return selectTarget.isBoolean();
}

vector<Selectable> QueryObject::getSelectables() {
    return selectTarget.getSelectable();
}

void QueryObject::cleanUp() {
    for (PatternClause pattern: patternClauses) {
        pattern.cleanUp();
    }
    for (auto declaration: declarations) {
        declaration.cleanUp();
    }
    for (auto cl: superClauses) {
        delete cl;
    }
}

Entities *QueryDeclaration::getType() const {
    return type;
}

string QueryDeclaration::getSynonym() const {
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
    cout << superClauses.size() << endl;
    for (SuperClause *cl: superClauses) {
        cout << cl->toString() << endl;
    }
}

bool QueryObject::hasUseOfUndeclaredVariable() const {
    return useOfUndeclaredVariable;
}

void QueryObject::setUseOfUndeclaredVariable(bool b) {
    useOfUndeclaredVariable = b;
}

vector<QueryDeclaration> QueryObject::getSelectablesAsQDs() {
    vector<QueryDeclaration> ret;
    for (Selectable &s: selectTarget.getSelectable()) {
        ret.push_back(s.getSynonym());
    }
    return ret;
}

SuperClause *QueryObject::popClause() {
    return superClauses.at(currPtr++);
}

size_t QueryObject::currGroupSize() {
    // only one group since there is no grouping in an unoptimized query object
    return superClauses.size();
}

bool QueryObject::currGroupCanSimplify() {
    return false;
}

bool QueryObject::isLastOfGroup() {
    return currPtr == superClauses.size() - 1;
}

bool QueryObject::empty() {
    return currPtr >= superClauses.size();
}
