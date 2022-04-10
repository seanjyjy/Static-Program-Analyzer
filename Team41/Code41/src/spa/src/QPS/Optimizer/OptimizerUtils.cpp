#include "OptimizerUtils.h"

/**
 * Removes duplicate clauses, e.g Calls(p, q), Calls(2, 3), Calls(p, q) -> Calls(p, q), Calls(2,3).
 * Preserves original positioning of clauses relative to each other.
 */
vector<SuperClause *> OptimizerUtils::removeDuplicates(const vector<SuperClause *> &clauses) {
    unordered_set<SuperClause> seen;
    vector<SuperClause *> ret;
    for (SuperClause *cl: clauses) {
        if (seen.find(*cl) == seen.end()) {
            ret.push_back(cl);
            seen.insert(*cl);
        }
    }
    return ret;
}

void OptimizerUtils::print(vector<vector<SuperClause *>> &clauses) {
    for (auto &arr: clauses) {
        cout << "array " << endl;
        for (auto &cl: arr) {
            cl->isAffectsT();
            cout << cl->toString() << " ";
        }
        cout << endl;
    }
}

bool OptimizerUtils::hasSynonymOverlap(const vector<QueryDeclaration> &qds, const vector<SuperClause *> &clauses) {
    unordered_set<string> synonyms;
    synonyms.reserve(qds.size()); // minor optimization

    for (const QueryDeclaration &qd: qds) synonyms.insert(qd.getSynonym());

    for (SuperClause *cl: clauses) {
        for (const QueryDeclaration &syn: cl->getSynonyms()) {
            if (synonyms.find(syn.getSynonym()) != synonyms.end()) {
                return true;
            }
        }
    }

    return false;
}
