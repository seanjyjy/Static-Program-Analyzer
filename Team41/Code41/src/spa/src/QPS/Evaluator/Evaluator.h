#pragma once

#include "QPS/Table/Table.h"
#include "QPS/Table/FalseTable.h"
#include "QPS/Table/TrueTable.h"
#include "QPS/Table/PQLTable.h"
#include "PKB/PKBClient.h"
#include "QPS/Adapters/NextKBAdapter.h"
#include "QPS/Adapters/AffectsKBAdapter.h"
#include "QPS/ClauseVariable.h"
#include "QPS/PatternClause.h"
#include "QPS/QueryClause.h"
#include "QPS/WithClause.h"
#include "QPS/Evaluator/EvaluatorUtils.h"
#include "Exception/SemanticException.h"

using namespace std;

class Evaluator {
private:
    Table *buildSingleSynonymTable(const unordered_set<string> &results, const string &label,
                                   QueryDeclaration::design_entity_type type);

protected:
    // ======================================= Generic TABLE BUILDING ==============================================
    Table *buildBooleanTable(bool booleanResult);

    Table *buildSingleSynonymTable(const unordered_set<string> &results, QueryDeclaration &patternSynonym);

    Table *buildSingleSynonymTable(const unordered_set<string> &results, ClauseVariable &synonym);

    Table *buildSingleSynonymTable(const string &result, ClauseVariable &synonym);

    Table *buildSingleSynonymTable(const vector<CFGNode *> &results, ClauseVariable &synonym);

    Table *buildSingleSynonymTable(const vector<string> &results, ClauseVariable &synonym);

    Table *buildSynonymSynonymTable(const vector<pair<string, string>> &results, ClauseVariable &leftSynonym,
                                    ClauseVariable &rightSynonym);

    Table *buildSameSynonymTable(const vector<pair<string, string>> &results, ClauseVariable &synonym);

    Table *buildDifferentSynonymTable(const vector<pair<string, string>> &results, ClauseVariable &leftSynonym,
                                      ClauseVariable &rightSynonym);

    Table *buildSynonymSynonymPatternTable(const vector<pair<string, string>> &results, QueryDeclaration &patternSyn,
                                           const ClauseVariable &left);

    unordered_set<string> getFilters(QueryDeclaration::design_entity_type);

public:
    PKBClient *pkb;

    explicit Evaluator(PKBClient *pkb);

    /**
     * Based on the clause, it will determine which evaluator to use to retrieve information from the pkb
     * to build the resultant table.
     *
     * @param clause A QueryClause that contains the clause type, and two clause variable.
     * @param pkb A knowledge base based on the Source Program.
     * @return A table that contains information based on the query that was executed.
     */

    friend class GenericClauseEvaluator;

    friend class PatternEvaluator;
};