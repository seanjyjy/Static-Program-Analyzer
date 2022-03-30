#pragma once

#include "QPS/Entities/Readers/EntitiesReader.h"

class StmtReader : public EntitiesReader {
public:
    ~StmtReader() override;

    long long getRowcount(PKBClient *pkb) const override;

    unordered_set<string> getEntities(PKBClient *pkb) override;
};
