#pragma once
#include "QPS/Table/Table.h"
#include "QPS/Table/FalseTable.h"
#include "QPS/Table/TrueTable.h"
#include "QPS/Table/PQLTable.h"
#include "PKB/PKB.h"
#include <string>

using namespace std;

class Evaluator {
public:
    // string to be changed once a clause side is created
    Table* evaluate(string clause, PKB *pkb);
};