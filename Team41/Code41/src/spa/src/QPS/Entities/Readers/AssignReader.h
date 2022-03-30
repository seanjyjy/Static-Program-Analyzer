#pragma once

#include "EntitiesReader.h"

class AssignReader : public EntitiesReader {
public:
    ~AssignReader() override;

    long long getRowcount(PKBClient *pkb) const override;

    unordered_set<string> getEntities(PKBClient *pkb) override;
};