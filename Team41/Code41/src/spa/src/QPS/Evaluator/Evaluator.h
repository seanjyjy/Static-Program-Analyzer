#pragma once
#include "QPS/Table/Table.h"
#include "QPS/Table/FalseTable.h"
#include "QPS/Table/TrueTable.h"
#include "QPS/Table/PQLTable.h"
#include "PKB/PKBClient.h"
#include "QPS/Adapters/NextKBAdapter.h"
#include "QPS/ClauseVariable.h"
#include "QPS/QueryClause.h"
#include "QPS/Evaluator/EvaluatorUtils.h"
#include "Exception/SemanticException.h"

using namespace std;

class Evaluator {
    PKBClient* pkb;
public:
    explicit Evaluator(PKBClient* pkb);

    /**
     * Based on the clause, it will determine which evaluator to use to retrieve information from the pkb
     * to build the resultant table.
     *
     * @param clause A QueryClause that contains the clause type, and two clause variable.
     * @param pkb A knowledge base based on the Source Program.
     * @return A table that contains information based on the query that was executed.
     */
    virtual Table* evaluate(QueryClause clause) = 0;

    string getClauseType(QueryClause::clause_type clauseType);

    // ================================================ TABLE BUILDING ==============================================
    Table* buildBooleanTable(bool booleanResult);
    Table* buildBooleanTable(unordered_set<string> &results);
    Table* buildBooleanTable(const string &result);
    Table* buildBooleanTable(vector<pair<string, string>> results);

    Table* buildSingleSynonymTable(unordered_set<string> &results, ClauseVariable &synonym);
    Table* buildSingleSynonymTable(const string &result, ClauseVariable &synonym);
    Table* buildSynonymSynonymTable(vector<pair<string, string>> &results, ClauseVariable &leftSynonym,
                                    ClauseVariable &rightSynonym);
    Table* buildSameSynonymTable(vector<pair<string, string>> &results, ClauseVariable &synonym;

    Table* buildDifferentSynonymTable(vector<pair<string, string>> &results, ClauseVariable &leftSynonym,
                                      ClauseVariable &rightSynonym);

    unordered_set<string> getFilters(QueryDeclaration::design_entity_type);

    friend class GenericClauseEvaluator;
    friend class PatternEvaluator;
};