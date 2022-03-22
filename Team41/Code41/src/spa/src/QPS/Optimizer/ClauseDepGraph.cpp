#include "ClauseDepGraph.h"
#include "TempClause.h"
#include "QPS/ClauseVariable.h"
#include <stdexcept>
#include <utility>

using namespace std;

ClauseDepGraph::ClauseDepGraph(PKBAdapter adapter): pkbAdapter(std::move(adapter)) {
}

void ClauseDepGraph::registerClause(TempClause qc) {
    vector<ClauseVariable> synonyms = qc.getSynonyms();

    // clauses without synonyms go into one group
    if (synonyms.empty()) {
        synonymToClauses[NO_SYNONYM].push_back(qc);
        return;
    }
    int len = synonyms.size();
    for (int i = 0; i < len; i++) {
        if (i != 0) graph.addEdge(synonyms[i-1].getLabel(), synonyms[i].getLabel());
        synonymToClauses[synonyms[i].getLabel()].push_back(qc);
    }
}

ClauseGroups ClauseDepGraph::split() {
    vector<vector<string>> groups = graph.getDisjointComponents();
    int N = (int) groups.size();

    // populate clause groups based on components
    ClauseGroups clauseGroups(N, pkbAdapter);
    for (int gid = 0; gid < N; gid++) { // loop all groups of synonyms
        int len = groups[gid].size();
        for (int cid = 0; cid < len; cid++) { // loop one group of syns
            for (const TempClause &cl: getClausesOfSyn(groups[gid][cid])) {
                clauseGroups.addClause(gid, cl);
            }
        }
    }

    // populate the final group - with no clauses
    ClauseGroup noSynClauses(pkbAdapter);
    for (const TempClause &tc: synonymToClauses[NO_SYNONYM]) {
        noSynClauses.addClause(tc);
    }
    clauseGroups.addClauseGroup(noSynClauses);

    return clauseGroups;
}

bool ClauseDepGraph::hasSyn(const string &syn) {
    return synonymToClauses.find(syn) != synonymToClauses.end();
}

vector<TempClause> ClauseDepGraph::getClausesOfSyn(const string &syn) {
    if (!hasSyn(syn)) throw runtime_error("syn does not exist");
    return synonymToClauses[syn];
}
