#pragma once

#include "QPS/QueryClause.h"
#include "QPS/PatternClause.h"

class TempClause {
    QueryClause* qc = nullptr;
    PatternClause* pc = nullptr;
public:
    TempClause(QueryClause *qc);
    TempClause(PatternClause *pc);
    vector<ClauseVariable> getSynonyms();
    bool hasSynonyms() const;
    bool isWithClause() const;
    bool isSuchThatClause() const;
    bool isPatternClause() const;
    bool isFollows() const;
    bool isFollowsT() const;
    bool isParent() const;
    bool isParentT() const;
    bool isUsesS() const;
    bool isUsesP() const;
    bool isModifiesS() const;
    bool isModifiesP() const;
    bool isCalls() const;
    bool isCallsT() const;
    bool isNext() const;
    bool isNextT() const;
    bool isAffects() const;
    bool isAffectsT() const;
};
