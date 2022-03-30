#pragma once

#include "EntitiesReader.h"

class WhileReader : public EntitiesReader {
public:
    ~WhileReader() override;

    long long getRowcount(PKBClient *pkb) const override;

    unordered_set<string> getEntities(PKBClient *pkb) override;
};