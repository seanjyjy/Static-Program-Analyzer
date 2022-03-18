#include "QueryProjector.h"

#include <utility>
#include "Evaluator/EvaluatorUtils.h"
#include "Exception/SemanticException.h"

QueryProjector::QueryProjector(SelectTarget select, Table *table, PKBClient *pkb, bool isValid) :
        selectTarget(move(select)), table(table), pkb(pkb), isValid(isValid) {}

QueryProjector::~QueryProjector() {
    delete table;
}

unordered_set<string> QueryProjector::getResult() {
    if (!this->isValid) return {};
    if (this->selectTarget.isBoolean()) return getBooleanResult();
    if (this->selectTarget.isTuple()) return getTupleResult();
}

unordered_set<string> QueryProjector::getBooleanResult() {
    if (table->isEmpty()) {
        // Occurs when result table is a false table or pql table which is empty
        return {"FALSE"};
    }
    return {"TRUE"};
}

unordered_set<string> QueryProjector::getTupleResult() {
    vector<Selectable> selectables = selectTarget.tuple;
    unordered_set<string> results;
    for (auto row: table->getRows()) {
        string result;
        for (Selectable target: selectables) {
            result.append(getProjectionFromRow(row, &target)).append(" ");
        }
        results.insert(result.substr(0, result.size() - 1));
    }
    return results;
}

string QueryProjector::getProjectionFromRow(const Row* row, Selectable* target) {
    QueryDeclaration declaration = target->getSynonym();
    string rawData = row->getValueAtColumn(declaration.synonym);
    if (target->getType() == target->SYNONYM) {
        return rawData;
    }

    optional<string> result = EvaluatorUtils::AttrUtils::getAttrFromSelectable(target, rawData, pkb);
    if (result == nullopt) {
        throw SemanticException("Invalid attribute for declaration: " + declaration.synonym);
    }
    return *result;
}
