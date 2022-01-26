//
// Created by lum jian yang sean on 26/1/22.
//
#pragma once

#include <set>
#include <string>
#include <vector>
#include "Evaluator.h"

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
    std::set<std::string> evaluateQuery(QueryObject *queryObject);
    Evaluator* getEvaluator(const std::string& clause);
};
