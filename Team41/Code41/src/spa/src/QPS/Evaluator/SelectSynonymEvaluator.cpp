#include "SelectSynonymEvaluator.h"
#include <stdexcept>

Table *SelectSynonymEvaluator::evaluate(const QueryDeclaration& selectSynonym) {
    auto synonym = selectSynonym.getSynonym();

    EntitiesReader* reader = selectSynonym.getType()->getReader();
    unordered_set<string> setOfResult = reader->getEntities(pkb);
    Header header({synonym});
    Table *resultTable = new PQLTable(header);

    for (auto &result: setOfResult) {
        Row *row = new Row(synonym, result);
        resultTable->addRow(row);
    }

    return resultTable;
}

SelectSynonymEvaluator::SelectSynonymEvaluator(PKBClient *pkb) {
    this->pkb = pkb;
}
