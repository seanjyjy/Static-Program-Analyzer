#include "ClauseDepGraph.h"
#include "QPS/SuperClause.h"
#include "QPS/ClauseVariable.h"
#include <stdexcept>

using namespace std;

ClauseDepGraph::ClauseDepGraph(PKBAdapter adapter) : pkbAdapter(adapter) {
}

void ClauseDepGraph::registerClause(SuperClause *cl) {
  vector<QueryDeclaration> synonyms = cl->getSynonyms();

  // clauses without synonyms go into one group
  if (synonyms.empty()) {
	synonymToClauses[NO_SYNONYM].push_back(cl);
	return;
  }

  // if the clause has synonyms, connect all pairs of synonyms
  for (int i = 0; i < synonyms.size(); i++) {
	for (int j = i + 1; j < synonyms.size(); j++) {
	  graph.addUndirectedEdge(synonyms[i].getSynonym(), synonyms[j].getSynonym());
	}
	// map clause synonyms to the original clause, so we can retrieve them later
	synonymToClauses[synonyms[i].getSynonym()].push_back(cl);
  }
}

ClauseGroups ClauseDepGraph::split() {
  vector<vector<string>> groups = graph.getDisjointComponents();
  int N = (int)groups.size();

  // populate clause groups based on components
  ClauseGroups clauseGroups(N, pkbAdapter);
  for (int gid = 0; gid < N; gid++) { // loop all groups of synonyms
	for (int cid = 0; cid < groups[gid].size(); cid++) { // loop one group of syns
	  for (SuperClause *cl : getClausesOfSyn(groups[gid][cid])) {
		clauseGroups.addClause(gid, cl);
	  }
	}
  }

  // populate the final group - with no clauses
  ClauseGroup noSynClauses(pkbAdapter);
  for (SuperClause *tc : synonymToClauses[NO_SYNONYM]) {
	noSynClauses.addClause(tc);
  }
  clauseGroups.addClauseGroup(noSynClauses);

  return clauseGroups;
}

bool ClauseDepGraph::hasSyn(const string &syn) {
  return synonymToClauses.find(syn) != synonymToClauses.end();
}

vector<SuperClause *> ClauseDepGraph::getClausesOfSyn(const string &syn) {
  if (!hasSyn(syn)) throw runtime_error("syn does not exist");
  return synonymToClauses[syn];
}
