#pragma once

#include <functional>
#include <queue>

#include "QPS/Cache/Cache.h"
#include "Common/CFGNode.h"
#include "PKB/PKBClient.h"

typedef std::function<bool(const string &)> CacheCallback;
typedef std::function<bool(CFGNode *)> TerminateCheck;

/**
 * Utility class to provide common algorithms that are being used in Next and Affects
 */
class AdaptersUtils {
private:
public:
    /**
     * A bfs algorithm that is a generic and reusable in other BFS logic.
     *
     * @param isForward Bool that decides whether to use children or parent in the CFGNode for next nodes to visit.
     * @param cacheAndContinue Function that cache results and returns a boolean to continue or stop.
     * @param canTerminate Function that returns true if can terminate in current iteration else return false.
     * @param startNode Starting CFGNode to start traversal from.
     */
    static void runBFS(bool isForward, CFGNode *startNode, const CacheCallback &cacheAndContinue,
                       const TerminateCheck &canTerminate = [](CFGNode *) { return false; });

    /**
     * A bfs algorithm that find is there exist a path from start to end. Utilizes runBFS.
     *
     * @param start Starting label of the CFGNode to start from.
     * @param end Ending label of the CFGNode to reach and end.
     * @param cache An instance of the cache to store the relation that is required in this relation.
     * @param node Starting CFGNode to start traversal from.
     */
    static void runBoolBFS(const string &start, const string &end, Cache *cache, CFGNode *node);

    /**
     * A bfs algorithm that runs in a downward motion given a starting node and will only terminate when all nodes that
     * are reachable are visited.
     *
     * @param stmtNum Starting label of the CFGNode to start from.
     * @param cache An instance of the cache to store the relation that is required in this relation.
     * @param node Starting CFGNode to start traversal from.
     */
    static void runDownBFS(const string &stmtNum, Cache *cache, CFGNode *node);

    /**
     * A bfs algorithm that runs in a upward motion (i.e moving to its parent's node) given a starting node
     * and will only terminate when all nodes that are reachable are visited.
     *
     * @param stmtNum Starting label of the CFGNode to start from.
     * @param cache An instance of the cache to store the relation that is required in this relation.
     * @param node Starting CFGNode to start traversal from.
     */
    static void runUpBFS(const string &stmtNum, Cache *cache, CFGNode *node);

    /**
     * Runs BFS on all nodes that are visited from the given node.
     *
     * @param cache An instance of the cache to store the relation that is required in this relation.
     * @param node Starting CFGNode to start traversal from.
     */
    static void fullBFS(Cache *cache, CFGNode *node);

    /**
     * Gets the starting node representing the current group/procedure. An example would be given a procedure which has
     * statement number 1 - 10 and given statement number 5, CFGNode representing statement number 1 will be returned.
     *
     * @param rootCFG Root CFGNode of all group/procedure.
     * @param stmt Current CFGNode's stmt number.
     * @return Returns the CFGNode that is representing the current group/procedure.
     */
    static CFGNode *getStartingParentNode(CFGNode *rootCFG, const string &stmt);
};