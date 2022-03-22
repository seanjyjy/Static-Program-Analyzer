#include "ClauseDepGraph.h"
#include "QPS/SuperClause.h"
#include "QPS/ClauseVariable.h"
#include <stdexcept>

using namespace std;

ClauseDepGraph::ClauseDepGraph() = default;

void ClauseDepGraph::registerClause(SuperClause *cl) {
    vector<QueryDeclaration> synonyms = cl->getSynonyms();

    // clauses without synonyms go into one group
    if (synonyms.empty()) {
        synonymToClauses[NO_SYNONYM].push_back(cl);
        return;
    }

    // if the clause has synonyms, connect all pairs of synonyms
    for (int i = 0; i < (int) synonyms.size(); i++) {
        for (int j = i + 1; j < (int) synonyms.size(); j++) {
            graph.addUndirectedEdge(synonyms[i].getSynonym(), synonyms[j].getSynonym());
        }
        // map clause synonyms to the original clause, so we can retrieve them later
        synonymToClauses[synonyms[i].getSynonym()].push_back(cl);
    }
}

vector<vector<SuperClause *>> ClauseDepGraph::split() {
    vector<vector<SuperClause *>> ret;

    // populate the first group - with no synonyms
    ret.push_back(synonymToClauses[NO_SYNONYM]);

    // populate all other groups based on components
    vector<vector<string>> groups = graph.getDisjointComponents();
    for (vector<string> &group: groups) { // for each disjoint group of synonyms
        vector<SuperClause *> clauseGroup;
        for (string &synonym: group) { // convert each synonym into its list of clauses
            clauseGroup.insert(clauseGroup.end(), synonymToClauses[synonym].begin(), synonymToClauses[synonym].end());
        }
        ret.push_back(clauseGroup);
    }

    return ret;
}
