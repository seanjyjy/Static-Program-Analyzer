#include "QueryObject.h"

#include <utility>

QueryObject::QueryObject(vector<QueryDeclaration> declarations,
                         vector<QueryClause> clauses,
                         vector<PatternClause> patternClauses,
                         QueryDeclaration selectSynonym,
                         SelectTarget selectTarget,
                         bool isQueryValid) :
    declarations(std::move(declarations)),
    clauses(std::move(clauses)),
    patternClauses(std::move(patternClauses)),
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
