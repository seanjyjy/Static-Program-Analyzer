#pragma once

#include "EntitiesReader.h"

class ConstantReader : public EntitiesReader {
public:
    ~ConstantReader() override;

    long long getRowcount(PKBClient* pkb) const override;
    unordered_set<string> getEntities(PKBClient* pkb) override;
};
