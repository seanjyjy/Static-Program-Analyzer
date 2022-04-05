#pragma once

#include "Common/AstNode/TNode.h"

using namespace std;

class PatternVariable {
public:
    // enum representing possible types of the pattern variable
    enum pattern_type {
        wildcard, fullpattern, subpattern
    };

    /**
     * Checks if the variable is a wildcard.
     *
     * @return boolean indicating if variable is a wildcard.
     */
    bool isWildcard();

    /**
     * Checks if the AST is a full pattern.
     *
     * @return boolean indicating if the AST is a full pattern.
     */
    bool isFullPattern();

    /**
     * Checks if the AST is a sub pattern.
     *
     * @return boolean indicating if the AST is a sub pattern.
     */
    bool isSubPattern();

    /**
     * Getter for the AST in the instance.
     *
     * @return TNode * pointing to the AST representation of the query.
     */
    TNode *getMiniAST();

    /**
     * Clean up method to delete AST
     */
    void cleanAST();

    /**
     * Constructor for the PatternVariable class.
     *
     * @param type of the new pattern variable
     * @param miniAST representing the parsed SIMPLE code
     */
    PatternVariable(pattern_type type, TNode *miniAST, string expr);

    PatternVariable(pattern_type type, TNode *miniAST);

    bool equals(PatternVariable) const;

    string getExpr() const;

    pattern_type getType() const;

private:
    // parsed AST to be compared to the main SIMPLE AST
    TNode *miniAST;

    // type of this pattern variable instance
    pattern_type type;

    string expr;
};
