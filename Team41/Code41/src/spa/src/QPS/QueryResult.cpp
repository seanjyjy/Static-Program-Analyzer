#include "QueryResult.h"

QueryResult::QueryResult(SelectTarget select, Table *table, PKBClient *pkb, bool isValid) :
        selectTarget(move(select)), table(table), pkb(pkb), isValidQuery(isValid) {}

bool QueryResult::isBoolean() {
    return selectTarget.isBoolean();
}

bool QueryResult::isTuple() {
    return selectTarget.isTuple();
}

bool QueryResult::isEmpty() {
    return table->isEmpty();
}

bool QueryResult::isValid() const {
    return isValidQuery;
}

vector<Selectable> QueryResult::getSelectables() {
    return selectTarget.getSelectable();
}

Table *QueryResult::getTable() {
    return table;
}

PKBClient *QueryResult::getPKB() {
    return pkb;
}
