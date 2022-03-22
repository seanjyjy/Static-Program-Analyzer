#pragma once

#include "AbstractGroups.h"
#include "AbstractGroup.h"

class FifoGroups : public AbstractGroups {
private:
    vector<AbstractGroup *> groups;
    vector<AbstractGroup *>::iterator it;
public:
    FifoGroups();

    void addGroup(AbstractGroup *group) override;

    SuperClause *pop() override;

    bool empty() override;

    AbstractGroup *front() override;

    string toString() const override;

    void print() const override;
};
