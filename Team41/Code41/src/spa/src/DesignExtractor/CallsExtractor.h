#pragma once

#include <list>
#include <unordered_set>
#include <unordered_map>

#include "Base/BaseExtractor.h"
#include "Common/AstNode/TNode.h"

class CallsExtractor : BaseExtractor {
private:
    const unordered_set<string> &procSet; // set of procedure names
    unordered_map<string, unordered_set<string>> callsMap; // mapping of proc to set of proc it directly calls
    unordered_map<string, unordered_set<string>> callsTMap; // mapping of proc to set of proc it callsT
    list<string> procCallOrder; // list of procedures in reversed toposort order of calls graph

    /**
     * Records proc calls proc into callsMap.
     *
     * @param procCalled procedure name that is being called
     * @param procCaller procedure name that call stmt is inside
     * @throws SemanticException if procedure being called doesn't exist
     */
    void mapCalls(const string &procCalled, const string &procCaller);

    /**
     * Traverses through AST, building calls mapping.
     *
     * @param node current TNode being processed in tree traversal
     * @param procCaller name of procedure that node is in
     */
    void dfs(TNode *node, const string &proc);

    /**
     * Checks if there is cyclic procedure calls.
     * Calls cycleCheckDFS which throw SemanticException if cycle detected.
     */
    void cycleCheck();

    /**
     * Traverses procedure calls graph through DFS to detect cyclic procedure calls.
     *
     * @param proc Current procedure being processed
     * @param visMap Map of procedure to int which tracks visited state of procedures (UNVISITED = 0, EXPLORED = 1, VISITED = 2)
     * @throws SemanticException if cyclic procedure call detected
     */
    void cycleCheckDfs(const string &proc, unordered_map<string, int> &visMap);

    /**
     * Builds procCallOrder in reverse TopoSort order of Calls graph.
     */
    void revTopoSort();

    /**
     * Dfs traversal of Calls graph for TopoSort.
     *
     * @param proc Current procedure being processed
     * @param visSet Set of procedures visited in Calls graph
     */
    void topoSort(const string &proc, unordered_set<string> &visSet);

    /**
     * Builds proc callsT proc mapping, using callsMap and procCallOrder.
     */
    void buildCallsT();

public:
    CallsExtractor(TNode *ast, unordered_set<string> &procSet);

    /**
     * Records all calls and callsT relationship in callsMap and callsTMap.
     *
     * @return True if extraction successful, False if Semantic Exception encountered
     */
    bool extract() override;

    unordered_map<string, unordered_set<string>> getCallsMap();

    unordered_map<string, unordered_set<string>> getCallsTMap();

    list<string> getProcCallOrder();
};