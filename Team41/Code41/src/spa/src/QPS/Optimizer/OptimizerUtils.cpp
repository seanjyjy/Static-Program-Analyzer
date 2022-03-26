#include "OptimizerUtils.h"

/**
 * Removes duplicate clauses, e.g Calls(p, q), Calls(2, 3), Calls(p, q) -> Calls(p, q), Calls(2,3).
 * Preserves original positioning of clauses relative to each other.
 */
vector<SuperClause *> OptimizerUtils::removeDuplicates(const vector<SuperClause *> &clauses) {
    unordered_set<SuperClause> seen;
    vector<SuperClause*> ret;
    for (SuperClause* cl: clauses) {
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
