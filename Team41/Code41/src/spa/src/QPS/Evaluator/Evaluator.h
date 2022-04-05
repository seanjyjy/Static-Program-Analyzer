#pragma once

#include "QPS/Table/Table.h"
#include "QPS/Table/FalseTable.h"
#include "QPS/Table/TrueTable.h"
#include "QPS/Table/PQLTable.h"
#include "PKB/PKBClient.h"
#include "QPS/Proxy/NextKBProxy.h"
#include "QPS/Proxy/AffectsKBProxy.h"
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
                                   Entities *type, bool canSimplify);

protected:
    // ======================================= Generic TABLE BUILDING ==============================================
    Table *buildBooleanTable(bool booleanResult);

    Table *buildSingleSynonymTable(const unordered_set<string> &results, QueryDeclaration &patternSynonym,
                                   bool canSimplify = false);

    Table *buildSingleSynonymTable(const unordered_set<string> &results, ClauseVariable &synonym,
                                   bool canSimplify = false);

    Table *buildSingleSynonymTable(const string &result, ClauseVariable &synonym, bool canSimplify = false);

    Table *buildSingleSynonymTable(const vector<CFGNode *> &results, ClauseVariable &synonym, bool canSimplify = false);

    Table *buildSingleSynonymTable(const vector<string> &results, ClauseVariable &synonym, bool canSimplify = false);

    Table *buildSynonymSynonymTable(const vector<pair<string, string>> &results, ClauseVariable &leftSynonym,
                                    ClauseVariable &rightSynonym, bool canSimplify = false);

    Table *buildSameSynonymTable(const vector<pair<string, string>> &results, ClauseVariable &synonym,
                                 bool canSimplify);

    Table *buildDifferentSynonymTable(const vector<pair<string, string>> &results, ClauseVariable &leftSynonym,
                                      ClauseVariable &rightSynonym, bool canSimplify);

    Table *buildSynonymSynonymPatternTable(const vector<pair<string, string>> &results, QueryDeclaration &patternSyn,
                                           const ClauseVariable &left);

public:
    PKBClient *pkb;

    explicit Evaluator(PKBClient *pkb);

    friend class GenericClauseEvaluator;

    friend class PatternEvaluator;
};