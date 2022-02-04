#pragma once

#include <set>
#include <string>
#include <vector>

#include "PKB/PKB.h"
#include "QPS/Evaluator/Evaluator.h"
#include "QPS/Evaluator/UsesSEvaluator.h"
#include "QPS/Evaluator/UsesPEvaluator.h"

// just a stub
class QueryObject{
public:
    // perhaps instead of string we can have a declaration class? to represent all type of declarations
    std::vector<std::string> declarations;
    // assume string is clause
    std::vector<std::string> clauses;
    bool isQueryValid;
//    QueryObject() {
//        // assuming v is a variable
//        declarations.push_back("v");
//        clauses.push_back("Uses(2, v)");
//        isQueryValid = false;
//    }
};

class QueryEvaluator {
private:
    PKB* pkb;
public:
    explicit QueryEvaluator(PKB* pkb);
    std::set<std::string> evaluateQuery(QueryObject *queryObject);
    Table* evaluate(string clause);
};
