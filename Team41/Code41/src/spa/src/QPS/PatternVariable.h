#pragma once

#include "Common/TNode.h"

using namespace std;

class PatternVariable {
private:
    TNode *miniAST;
public:
    enum pattern_type {
        wildcard, fullpattern, subpattern
    };
    pattern_type type;

    bool isWildcard();
    bool isFullPattern();
    bool isSubPattern();
    TNode *getMiniAST();
    PatternVariable(pattern_type type, TNode *miniAST);
};
