#include "Evaluator.h"

Evaluator::Evaluator(PKBClient *pkb) {
    this->pkb = pkb;
}

unordered_set<string> Evaluator::getFilters(QueryDeclaration::design_entity_type types) {
    switch (types) {
        case QueryDeclaration::design_entity_type::ASSIGN:
            return pkb->getAssigns();
        case QueryDeclaration::design_entity_type::CALL:
            return pkb->getCalls();
        case QueryDeclaration::design_entity_type::CONSTANT:
            return pkb->getConstants();
        case QueryDeclaration::design_entity_type::IF:
            return pkb->getIfs();
        case QueryDeclaration::design_entity_type::PRINT:
            return pkb->getPrints();
        case QueryDeclaration::design_entity_type::PROCEDURE:
            return pkb->getProcedures();
        case QueryDeclaration::design_entity_type::READ:
            return pkb->getReads();
        case QueryDeclaration::design_entity_type::STMT:
            return pkb->getStatements();
        case QueryDeclaration::design_entity_type::VARIABLE:
            return pkb->getVariables();
        case QueryDeclaration::design_entity_type::WHILE:
            return pkb->getWhiles();
        default:
            throw runtime_error("NO SUCH DESIGN ENTITY TYPE AVAILABLE");
    }
}

Table *Evaluator::buildBooleanTable(bool booleanResult) {
    if (booleanResult) {
        return new TrueTable();
    }

    return new FalseTable();
}

Table *Evaluator::buildSingleSynonymTable(const string &result, ClauseVariable &synonym) {
    if (result.empty()) {
        return new FalseTable();
    }

    string column = synonym.getLabel();
    unordered_set<string> filters = getFilters(synonym.getDesignEntityType());

    Header header({column});
    Table *table = new PQLTable(header);

    if (filters.find(result) != filters.end()) {
        Row *row = new Row(column, result);
        table->addRow(row);
    }

    return table;
}

Table *Evaluator::buildSingleSynonymTable(const unordered_set<string> &results, QueryDeclaration &patternSynonym) {
    return buildSingleSynonymTable(results, patternSynonym.getSynonym(), patternSynonym.getType());
}

Table *Evaluator::buildSingleSynonymTable(const unordered_set<string> &results, ClauseVariable &synonym) {
    return buildSingleSynonymTable(results, synonym.getLabel(), synonym.getDesignEntityType());
}

Table *Evaluator::buildSingleSynonymTable(const unordered_set<string> &results, const string &label,
                                          QueryDeclaration::design_entity_type type) {

    if (results.empty()) {
        return new FalseTable();
    }

    unordered_set<string> filters = getFilters(type);

    Header header({label});
    Table *table = new PQLTable(header);

    for (auto &child: results) {
        if (filters.find(child) != filters.end()) {
            Row *row = new Row(label, child);
            table->addRow(row);
        }
    }

    return table;
}

Table *Evaluator::buildSingleSynonymTable(const vector<CFGNode *> &results, ClauseVariable &synonym) {
    if (results.empty()) {
        return new FalseTable();
    }

    string column = synonym.getLabel();
    unordered_set<string> filters = getFilters(synonym.getDesignEntityType());

    Header header = Header({column});
    Table *table = new PQLTable(header);

    for (auto &child: results) {
        if (filters.find(child->getStmtNum()) != filters.end()) {
            Row *row = new Row(column, child->getStmtNum());
            table->addRow(row);
        }
    }

    return table;
}

Table *Evaluator::buildSingleSynonymTable(const vector<string> &results, ClauseVariable &synonym) {
    if (results.empty()) {
        return new FalseTable();
    }

    string column = synonym.getLabel();
    unordered_set<string> filters = getFilters(synonym.getDesignEntityType());

    Header header = Header({column});
    Table *table = new PQLTable(header);

    for (auto &statement: results) {
        if (filters.find(statement) != filters.end()) {
            Row *row = new Row(column, statement);
            table->addRow(row);
        }
    }

    return table;
}

Table *Evaluator::buildSynonymSynonymTable(const vector<pair<string, string>> &results, ClauseVariable &leftSynonym,
                                           ClauseVariable &rightSynonym) {
    if (results.empty()) {
        return new FalseTable();
    }

    if (leftSynonym.getLabel() == rightSynonym.getLabel()) {
        return buildSameSynonymTable(results, leftSynonym);
    }

    return buildDifferentSynonymTable(results, leftSynonym, rightSynonym);
}

Table *Evaluator::buildSameSynonymTable(const vector<pair<string, string>> &results, ClauseVariable &synonym) {
    string label = synonym.getLabel();
    unordered_set<string> filters = getFilters(synonym.getDesignEntityType());

    Header header({label});
    Table *table = new PQLTable(header);

    for (auto&[leftSyn, rightSyn]: results) {
        if (leftSyn == rightSyn && filters.find(leftSyn) != filters.end()) {
            Row *row = new Row(label, leftSyn);
            table->addRow(row);
        }
    }

    return table;
}

Table *Evaluator::buildDifferentSynonymTable(const vector<pair<string, string>> &results, ClauseVariable &leftSynonym,
                                             ClauseVariable &rightSynonym) {
    string leftLabel = leftSynonym.getLabel();
    string rightLabel = rightSynonym.getLabel();
    unordered_set<string> leftFilters = getFilters(leftSynonym.getDesignEntityType());
    unordered_set<string> rightFilters = getFilters(rightSynonym.getDesignEntityType());

    Header header({leftLabel, rightLabel});
    Table *table = new PQLTable(header);

    for (auto&[leftSyn, rightSyn]: results) {
        bool isInLeftFilter = leftFilters.find(leftSyn) != leftFilters.end();
        bool isInRightFilter = rightFilters.find(rightSyn) != rightFilters.end();
        if (isInLeftFilter && isInRightFilter) {
            Row *row = new Row();
            row->addEntry(leftLabel, leftSyn);
            row->addEntry(rightLabel, rightSyn);
            table->addRow(row);
        }
    }

    return table;
}

Table *Evaluator::buildSynonymSynonymPatternTable(const vector<pair<string, string>> &results,
                                                  QueryDeclaration &patternSyn, const ClauseVariable &left) {
    string firstColumn = patternSyn.getSynonym();
    string secondColumn = left.getLabel();
    unordered_set<string> leftFilters = getFilters(patternSyn.getType());
    unordered_set<string> rightFilters = getFilters(left.getDesignEntityType());

    Header header({firstColumn, secondColumn});
    Table *table = new PQLTable(header);

    for (auto &[stmt, var]: results) {
        bool isInLeftFilter = leftFilters.find(stmt) != leftFilters.end();
        bool isInRightFilter = rightFilters.find(var) != rightFilters.end();
        if (isInLeftFilter && isInRightFilter) {
            Row *row = new Row();
            row->addEntry(firstColumn, stmt);
            row->addEntry(secondColumn, var);
            table->addRow(row);
        }
    }

    return table;
}