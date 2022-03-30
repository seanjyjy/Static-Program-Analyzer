#pragma once

#include "EntitiesReader.h"

class IfReader : public EntitiesReader {
public:
    ~IfReader() override;

    long long getRowcount(PKBClient *pkb) const override;

    unordered_set<string> getEntities(PKBClient *pkb) override;
};
