#pragma once

#include <vector>
#include <stdexcept>
#include "TNode.h"
#include "Common/AstNode/Procedure.h"

using namespace std;

/**
 * Represents program -> procedure+
 */
class Program : public TNode {
public:
    Program(const vector<Procedure *> &procedures);
    string toString() override;
    [[nodiscard]] bool isProgram() const override;
};
