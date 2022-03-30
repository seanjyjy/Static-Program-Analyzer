#include "ClauseDepGraph.h"
#include "QPS/SuperClause.h"
#include "QPS/ClauseVariable.h"
#include <stdexcept>

using namespace std;

ClauseDepGraph::ClauseDepGraph() = default;

void ClauseDepGraph::registerClause(SuperClause *cl) {
    // generate a unique id for this particular clause
    int id = getCid();
    vector<QueryDeclaration> synonyms = cl->getSynonyms();

    // clauses without synonyms go into one group
    if (synonyms.empty()) {
        synonymToClauses[NO_SYNONYM].push_back({cl, id});
        isCidProcessed[id] = false;
        return;
    }

    // if the clause has synonyms, connect all pairs of synonyms
    for (int i = 0; i < (int) synonyms.size(); i++) {
        string iSyn = synonyms[i].getSynonym();
        graph.addVertex(iSyn);
        for (int j = i + 1; j < (int) synonyms.size(); j++) {
            string jSyn = synonyms[j].getSynonym();
            graph.addVertex(jSyn);
            graph.addUndirectedEdge(iSyn, jSyn);
        }
        // map clause synonyms to the original clause, so we can retrieve them later
        synonymToClauses[synonyms[i].getSynonym()].push_back({cl, id});
        isCidProcessed[id] = false;
    }
}

vector<vector<SuperClause *>> ClauseDepGraph::split() {
    vector<vector<SuperClause *>> ret;

    // populate the first group - with no synonyms
    vector<TaggedSuperClause> noSyns = synonymToClauses[NO_SYNONYM];
    if (!noSyns.empty()) {
        ret.push_back(getUnprocessedClauses(noSyns));
    }

    // populate all other groups based on components
    vector<vector<string>> groups = graph.getDisjointComponents();
    for (vector<string> &group: groups) { // for each disjoint group of synonyms
        vector<SuperClause *> clauseGroup;
        // convert each synonym into its list of clauses
        for (string &synonym: group) {
            // remove duplicate clauses
            vector<SuperClause*> toInsert = getUnprocessedClauses(synonymToClauses[synonym]);
            clauseGroup.insert(clauseGroup.end(), toInsert.begin(), toInsert.end());
        }
        ret.push_back(clauseGroup);
    }

    return ret;
}

int ClauseDepGraph::getCid() {
    return clauseId++;
}

vector<SuperClause *> ClauseDepGraph::getUnprocessedClauses(vector<TaggedSuperClause> &clauses) {
    vector<SuperClause*> ret;
    for (auto &[cl, clId]: clauses) {
        if (!isCidProcessed[clId]) {
            isCidProcessed[clId] = true;
            ret.push_back(cl);
        }
    }
    return ret;
}
