#include "SelectSynonymEvaluator.h"
#include <stdexcept>

Table* SelectSynonymEvaluator::evaluate(const QueryDeclaration selectSynonym, PKB *pkb) {
    auto declarationType = selectSynonym.type;
    auto synonym = selectSynonym.synonym;

    unordered_set<string> setOfResult = getResultViaType(declarationType, pkb);
    Header header({synonym});
    Table* resultTable = new PQLTable(header);

    for (auto& result : setOfResult) {
        Row* row = new Row(synonym, result);
        resultTable->addRow(row);
    }

    return resultTable;
}

unordered_set<string> SelectSynonymEvaluator::getResultViaType(QueryDeclaration::design_entity_type type, PKB* pkb) {
    switch(type) {
        case QueryDeclaration::design_entity_type::STMT:
            return pkb->getStatements();
        case QueryDeclaration::design_entity_type::READ:
            return pkb->getReads();
        case QueryDeclaration::design_entity_type::PRINT:
            return pkb->getPrints();
        case QueryDeclaration::design_entity_type::CALL:
            return pkb->getCalls();
        case QueryDeclaration::design_entity_type::WHILE:
            return pkb->getWhiles();
        case QueryDeclaration::design_entity_type::IF:
            return pkb->getIfs();
        case QueryDeclaration::design_entity_type::ASSIGN:
            return pkb->getAssigns();
        case QueryDeclaration::design_entity_type::VARIABLE:
            return pkb->getVariables();
        case QueryDeclaration::design_entity_type::CONSTANT:
            return pkb->getConstants();
        case QueryDeclaration::design_entity_type::PROCEDURE:
            return pkb->getProcedures();
        default:
            throw SemanticException("Invalid query provided for Select");
    }
}