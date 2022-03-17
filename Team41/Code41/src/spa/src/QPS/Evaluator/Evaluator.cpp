#include "Evaluator.h"

Evaluator::Evaluator(PKBClient *pkb) {
    this->pkb = pkb;
}

string Evaluator::getClauseType(QueryClause::clause_type clauseType) {
    switch (clauseType) {
        case QueryClause::clause_type::follows:
            return "Follows";
        case QueryClause::clause_type::followsT:
            return "Follows*";
        case QueryClause::clause_type::parent:
            return "Parent";
        case QueryClause::clause_type::parentT:
            return "Parent*";
        case QueryClause::clause_type::usesS:
            return "UsesS";
        case QueryClause::clause_type::usesP:
            return "UsesP";
        case QueryClause::clause_type::modifiesS:
            return "ModifiesS";
        case QueryClause::clause_type::modifiesP:
            return "ModfiesP";
        case QueryClause::clause_type::calls:
            return "Calls";
        case QueryClause::clause_type::callsT:
            return "Calls*";
        default:
            throw std::runtime_error("unknown clause of type");
    }
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

Table *Evaluator::buildBooleanTable(const unordered_set<string> &results) {
    if (results.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *Evaluator::buildBooleanTable(const vector<pair<string, string>>& results) {
    if (results.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *Evaluator::buildBooleanTable(const string &result) {
    if (result.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *Evaluator::buildBooleanTable(const vector<CFGNode *> &results) {
    if (results.empty()) {
        return new FalseTable();
    }

    return new TrueTable();
}

Table *Evaluator::buildSingleSynonymTable(const string &result, ClauseVariable &synonym) {
    if (result.empty()) {
        return new FalseTable();
    }

    string column = synonym.getLabel();
    unordered_set<string> filters = getFilters(synonym.getDesignEntityType());

    Header header({column});
    Table* table = new PQLTable(header);

    if (filters.find(result) != filters.end()) {
        Row* row = new Row(column, result);
        table->addRow(row);
    }

    return table;
}

Table *Evaluator::buildSingleSynonymTable(const unordered_set<string> &results, ClauseVariable &synonym) {
    if (results.empty()) {
        return new FalseTable();
    }

    string column = synonym.getLabel();
    unordered_set<string> filters = getFilters(synonym.getDesignEntityType());

    Header header({column});
    Table* table = new PQLTable(header);

    for (auto& child : results) {
        if (filters.find(child) != filters.end()) {
            Row* row = new Row(column, child);
            table->addRow(row);
        }
    }

    return table;
}

Table *Evaluator::buildSingleSynonymTable(const vector<CFGNode *>& results, ClauseVariable &synonym) {
    string column = synonym.getLabel();
    unordered_set<string> filters = getFilters(synonym.getDesignEntityType());

    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& child : results) {
        if (filters.find(child->getStmtNum()) != filters.end()) {
            Row* row = new Row(column, child->getStmtNum());
            table->addRow(row);
        }
    }

    return table;
}

Table *Evaluator::buildSingleSynonymTable(const vector<string> results, ClauseVariable &synonym) {
    if (results.empty()) {
        return new FalseTable();
    }

    string column = synonym.getLabel();
    unordered_set<string> filters = getFilters(synonym.getDesignEntityType());

    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& statement : results) {
        if (filters.find(statement) != filters.end()) {
            Row* row = new Row(column, statement);
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
    Table* table = new PQLTable(header);

    for (auto&[leftSyn, rightSyn] : results) {
        if (leftSyn == rightSyn && filters.find(leftSyn) != filters.end()) {
            Row* row = new Row(label, leftSyn);
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
    Table* table = new PQLTable(header);

    for (auto&[leftSyn, rightSyn] : results) {
        if (leftFilters.find(leftSyn) != leftFilters.end() && rightFilters.find(rightSyn) != rightFilters.end()) {
            Row* row = new Row();
            row->addEntry(leftLabel, leftSyn);
            row->addEntry(rightLabel, rightSyn);
            table->addRow(row);
        }
    }

    return table;
}