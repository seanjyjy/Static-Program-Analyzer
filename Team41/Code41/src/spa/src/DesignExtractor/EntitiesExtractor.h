#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Base/BaseExtractor.h"
#include "Common/TNode.h"

class EntitiesExtractor : public BaseExtractor {
private:
    unordered_map<TNode *, string> nodeToStmtNumMap; // mapping of TNode* to statement number
    unordered_set<string> procSet; // set of procedure names
    unordered_set<string> varSet; // set of variable names
    unordered_set<string> constSet; // set of constants (numbers)

    /**
     * Records procedure names to procSet from children nodes of the root of AST.
     *
     * @throws SemanticException if multiple procedures with same names exist
     */
    void findProcedures();

    /**
     * Records current node into varSet if it's variable, constSet if constant and nodeToStmtNumMap if statement.
     *
     * @param node current TNode of AST to record
     * @param stmtNum last registered statement number
     */
    void recordEntity(TNode *node, int &stmtNum);

    /**
     * Traverses through AST to record statements, variable names and constants.
     *
     * @param node current TNode being processed in tree traversal
     * @param stmtNum last registered statement number
     */
    void dfs(TNode *node, int &stmtNum); // statement number, varName, const

public:
    EntitiesExtractor(TNode *ast);

    /**
     * Records procedure names, statements, variable names and constants into respective sets/maps.
     *
     * @return True if extraction successful, False if Semantic Exception encountered
     */
    bool extract() override;

    /**
     * @return Map of TNode* of AST to statement numbers
     */
    unordered_map<TNode *, string> getNodeToStmtNumMap();

    /**
     * @return Set of procedure names
     */
    unordered_set<string> getProcSet();

    /**
     * @return Set of variable names
     */
    unordered_set<string> getVarSet();

    /**
     * @return Set of constants (numbers in form of string)
     */
    unordered_set<string> getConstSet();
};
