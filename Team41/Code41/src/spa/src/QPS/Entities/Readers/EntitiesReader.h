#pragma once

#include <unordered_set>
#include <string>
#include "PKB/PKBClient.h"

using namespace std;

class EntitiesReader {
public:
    virtual ~EntitiesReader();

    virtual long long getRowcount(PKBClient* pkb) const = 0;
    virtual unordered_set<string> getEntities(PKBClient* pkb) = 0;

    friend class StmtReader;
    friend class ReadReader;
    friend class PrintReader;
    friend class CallReader;
    friend class WhileReader;
    friend class IfReader;
    friend class AssignReader;
    friend class VariableReader;
    friend class ConstantReader;
    friend class ProcedureReader;
};