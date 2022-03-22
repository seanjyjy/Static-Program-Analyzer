#include "SimpleGraph.h"

#include <stdexcept>

SimpleGraph::SimpleGraph() = default;

void SimpleGraph::addVertex(string n1) {
  int id = genId();
  nameToNodeId[n1] = id;
  nodeIdToName[id] = n1;
  adjList[id] = vector<int>();
}

void SimpleGraph::addUndirectedEdge(string n1, string n2) {
  if (!hasNode(n1)) addVertex(n1);
  if (!hasNode(n2)) addVertex(n2);
  int id1 = getNodeId(n1);
  int id2 = getNodeId(n2);
  pair<int, int> e1 = {id1, id2};
  pair<int, int> e2 = {id2, id1};
  if (seenEdges.find(e1) == seenEdges.end()) {
	seenEdges.insert(e1);
	adjList[id1].push_back(id2);
  }
  if (seenEdges.find(e2) == seenEdges.end()) {
	seenEdges.insert(e2);
	adjList[id2].push_back(id1);
  }
}

void SimpleGraph::clear() {
  nodeId = 0;
  seenEdges.clear();
  nameToNodeId.clear();
  nodeIdToName.clear();
  adjList.clear();
}

vector<vector<string>> SimpleGraph::getDisjointComponents() {
  int V = (int)adjList.size();
  // find connected components with DFS
  int tag = 0;
  vector<int> tags(V, -1);
  for (int i = 0; i < V; i++) {
	if (tags[i] == -1) dfs(i, tag++, tags);
  }

  // split components into buckets
  vector<vector<string>> components(tag, vector<string>());
  for (int i = 0; i < V; i++) {
	int bucket = tags[i];
	int nid = i;
	string nodeName = getName(nid);
	components[bucket].push_back(nodeName);
  }

  return components;
}

int SimpleGraph::getNodeId(string nodeStmt) {
  if (nameToNodeId.find(nodeStmt) == nameToNodeId.end()) return -1;
  return nameToNodeId[nodeStmt];
}

string SimpleGraph::getName(int nodeIdArg) {
  if (nodeIdToName.find(nodeIdArg) == nodeIdToName.end()) throw runtime_error("no name for given nodeId");
  return nodeIdToName[nodeIdArg];
}

int SimpleGraph::genId() {
  return nodeId++;
}

bool SimpleGraph::hasNode(string node) {
  // if a node is in the graph, it definitely has a mapping
  return nameToNodeId.find(node) != nameToNodeId.end();
}

void SimpleGraph::dfs(int u, int tag, vector<int> &tags) {
  tags[u] = tag;
  for (int v : adjList[u]) {
	if (tags[v] == -1) dfs(v, tag, tags);
  }
}
