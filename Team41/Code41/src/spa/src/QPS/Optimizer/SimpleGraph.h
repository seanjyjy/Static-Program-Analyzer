#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>

using namespace std;

// TODO make generic
class SimpleGraph {
private:
    int nodeId = 0;
    unordered_map<string, int> nameToNodeId;
    unordered_map<int, string> nodeIdToName;
    // TODO maybe define custom hash function for unordered_set
    set<pair<int, int>> seenEdges; // simple graphs can't have multi-edges
    unordered_map<int, vector<int>> adjList;

    int getNodeId(string node);

    string getName(int nodeId);

    int genId();

    bool hasNode(string node);

    void dfs(int u, int tag, vector<int> &tags);

public:
    SimpleGraph();

    void addVertex(string n1);

    void addUndirectedEdge(string n1, string n2);

    void clear();

    vector<vector<string>> getDisjointComponents();
};
