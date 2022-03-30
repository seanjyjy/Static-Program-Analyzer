#pragma once

#include "EntitiesReader.h"

class PrintReader : public EntitiesReader {
public:
    ~PrintReader() override;

    long long getRowcount(PKBClient *pkb) const override;

    unordered_set<string> getEntities(PKBClient *pkb) override;
};
