#include "OptimizerUtils.h"

vector<SuperClause *> OptimizerUtils::removeDuplicates(const vector<SuperClause *> &clauses) {
    // TODO: implement after superclause refactor to use composition
    return clauses;
}

void OptimizerUtils::print(vector<vector<SuperClause *>> &clauses) {
    for (auto &arr: clauses) {
        cout << "array" << endl;
        for (auto &cl: arr) { // TODO implement after superclause refactor to use composition
            cl->isAffectsT();
            cout << "wip" << " ";
        }
        cout << endl;
    }
}
