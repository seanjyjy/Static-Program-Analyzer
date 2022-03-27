#pragma once

class SimplifiableClause {
private:
    bool canSimplify = false;

public:
    bool canSimplifyClause() const;

    void setSimplified(bool canSimplify);
};
