#pragma once

#include <vector>
#include <stdexcept>
#include "Common/TNode.h"
#include "Common/AstNode/Procedure.h"

using namespace std;

class Program: public TNode {
public:
    Program(const vector<Procedure *>& procedures);
};
