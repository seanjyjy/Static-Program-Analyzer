#pragma once

#include "WithVariable.h"
#include "SimplifiableClause.h"
#include <vector>

using namespace std;

class WithClause : public SimplifiableClause {
private:
    WithVariable left, right;
public:
    WithClause();
    WithClause(WithVariable, WithVariable);
    WithVariable getLeft() const;
    WithVariable getRight() const;
    vector<QueryDeclaration> getSynonyms() const;
    bool hasSynonyms() const;

    int hash() const;
    bool equals(WithClause) const;
    string toString() const;
};
