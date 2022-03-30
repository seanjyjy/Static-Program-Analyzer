#pragma once

#include "EntitiesReader.h"

class CallReader : public EntitiesReader {
public:
    ~CallReader() override;

    long long getRowcount(PKBClient *pkb) const override;

    unordered_set<string> getEntities(PKBClient *pkb) override;
};

