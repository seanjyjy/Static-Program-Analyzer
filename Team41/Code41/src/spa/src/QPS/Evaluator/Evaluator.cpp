#include "Evaluator.h"

Evaluator::Evaluator(PKBClient *pkb) {
    this->pkb = pkb;
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
    EntitiesReader *entityReader = synonym.getDesignEntityType()->getReader();
    unordered_set<string> filters = entityReader->getEntities(pkb);

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
                                          Entities* type) {

    if (results.empty()) {
        return new FalseTable();
    }

    EntitiesReader *entityReader = type->getReader();
    unordered_set<string> filters = entityReader->getEntities(pkb);

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
    EntitiesReader *entityReader = synonym.getDesignEntityType()->getReader();
    unordered_set<string> filters = entityReader->getEntities(pkb);

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
    EntitiesReader *entityReader = synonym.getDesignEntityType()->getReader();
    unordered_set<string> filters = entityReader->getEntities(pkb);

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
    EntitiesReader *entityReader = synonym.getDesignEntityType()->getReader();
    unordered_set<string> filters = entityReader->getEntities(pkb);

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

    EntitiesReader *leftEntityReader = leftSynonym.getDesignEntityType()->getReader();
    EntitiesReader *rightEntityReader = rightSynonym.getDesignEntityType()->getReader();
    unordered_set<string> leftFilters = leftEntityReader->getEntities(pkb);
    unordered_set<string> rightFilters = rightEntityReader->getEntities(pkb);

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
    if (results.empty()) {
        return new FalseTable();
    }
    string firstColumn = patternSyn.getSynonym();
    string secondColumn = left.getLabel();
    EntitiesReader *leftEntityReader = patternSyn.getType()->getReader();
    EntitiesReader *rightEntityReader = left.getDesignEntityType()->getReader();
    unordered_set<string> leftFilters = leftEntityReader->getEntities(pkb);
    unordered_set<string> rightFilters = rightEntityReader->getEntities(pkb);

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