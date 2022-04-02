#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>

using namespace std;

/**
 * Represents a graph that takes in a generic type as vertex label.
 * @tparam T the type of the vertex label (e.g string/primitive numeric types)
 *
 * Unrelated note:
 * Code involving templates must be defined in the template class.
 * https://stackoverflow.com/questions/5237055/c-generic-class-link-errors
 */
template <typename T>
class SimpleGraph {
private:
    int nodeId = 0;
    unordered_map<T, int> nameToNodeId; // allow simpleGraph to work with integer representations
    unordered_map<int, T> nodeIdToName; // allow simpleGraph to work with integer representations
    set<pair<int, int>> seenEdges; // simple graphs can't have multi-edges
    unordered_map<int, vector<int>> adjList;

    // returns a unique id. allows the graph to represent nodes with integers under the hood.
    int genNodeId() {
        return nodeId++;
    }

    // checks if a node has already been added.
    bool hasNode(T node) {
        // if a node is in the graph, it definitely has a mapping
        return nameToNodeId.find(node) != nameToNodeId.end();
    }

    // performs dfs on each connected component, tagging each group with a unique tag. for connected cmpt generation.
    void dfs(int u, int tag, vector<int> &tags) {
        tags[u] = tag;
        for (int v: adjList[u]) {
            if (tags[v] == -1) dfs(v, tag, tags);
        }
    }
public:
    SimpleGraph() = default;

    /**
     * Adds a vertex to the graph. Adding the same vertex multiple times will not affect the final result.
     */
    void addVertex(T n1) {
        // don't add the same vertex more than one
        if (nameToNodeId.find(n1) != nameToNodeId.end()) return;
        int id = genNodeId();
        nameToNodeId[n1] = id;
        nodeIdToName[id] = n1;
        adjList[id] = vector<int>();
    }

    /**
     * Adds an undirected, unweighted edge between two vertices. Creates the vertex if a given vertex does not exist.
     * Ensures that only one edge may exist between two nodes.
     */
    void addUndirectedEdge(T n1, T n2) {
        if (!hasNode(n1)) addVertex(n1);
        if (!hasNode(n2)) addVertex(n2);
        int id1 = nameToNodeId[n1];
        int id2 = nameToNodeId[n2];
        pair<int, int> e1 = {id1, id2};
        pair<int, int> e2 = {id2, id1};
        // don't add the same edge more than once
        if (seenEdges.find(e1) == seenEdges.end()) {
            seenEdges.insert(e1);
            adjList[id1].push_back(id2);
        }
        // don't add the same edge more than once
        if (seenEdges.find(e2) == seenEdges.end()) {
            seenEdges.insert(e2);
            adjList[id2].push_back(id1);
        }
    }

    /**
     * Returns the disjoint components of the graph.
     */
    vector<vector<T>> getDisjointComponents() {
        int V = (int) adjList.size();
        // find connected components with DFS
        int tag = 0;
        vector<int> tags(V, -1);
        for (int i = 0; i < V; i++) {
            if (tags[i] == -1) dfs(i, tag++, tags);
        }

        // split components into buckets
        vector<vector<T>> components(tag, vector<T>());
        for (int i = 0; i < V; i++) {
            int bucket = tags[i];
            int nid = i;
            T nodeName = nodeIdToName[nid];
            components[bucket].push_back(nodeName);
        }

        return components;
    }
};
