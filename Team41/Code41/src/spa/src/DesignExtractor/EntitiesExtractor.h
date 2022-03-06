#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Common/TNode.h"

class EntitiesExtractor {
private:
    TNode *ast; // root node of AST
    unordered_map<TNode *, string> nodeToStmtNumMap; // mapping of TNode* to statement number
    unordered_map<string, list<string>> procCallMap; // mapping of proc to list of proc it directly calls
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
     * Records proc calls proc into procCallMap
     *
     * @param procCalled procedure name that is being called
     * @param procCaller procedure name that call stmt is inside
     */
    void recordProcCall(string procCalled, string &procCaller);

    /**
     * Records current node into varSet if it's variable, constSet if constant and nodeToStmtNumMap if statement.
     *
     * @param node current TNode of AST to record
     * @param stmtNum last registered statement number
     * @param procName name of procedure that node is in
     */
    void recordEntity(TNode *node, int &stmtNum, string &procName);

    /**
     * Traverses through AST to record statements, procedure calls, variable names and constants.
     */
    void findEntities(); // statement number, varName, const, procCallProc

public:
    EntitiesExtractor(TNode *ast);

    /**
     * Records procedure names, statements, procedure calls, variable names and constants into respective sets/maps.
     */
    void extractEntities();

    /**
     * @return Map of TNode* of AST to statement numbers
     */
    unordered_map<TNode *, string> getNodeToStmtNumMap();

    /**
     * @return Map of procedures to procedures it calls
     */
    unordered_map<string, list<string>> getProcCallMap();

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
