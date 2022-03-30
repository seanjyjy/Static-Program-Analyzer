#pragma once

#include "EntitiesReader.h"

class VariableReader : public EntitiesReader {
public:
    ~VariableReader() override;

    long long getRowcount(PKBClient* pkb) const override;
    unordered_set<string> getEntities(PKBClient* pkb) override;
};
