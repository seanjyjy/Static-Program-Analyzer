#pragma once

#include "Evaluator.h"
#include <functional>

// If required is a stmt, return back the stmt provided, else get its required mapping thru the function provided
typedef std::function<string(const string &, PKBClient *pkb)> ValueMapping;

class WithEvaluator : Evaluator {
private:
    bool isIntegerInteger(WithVariable &left, WithVariable &right);

    bool isValidIntegerAttrRef(WithVariable &left, WithVariable &right);

    bool isIdentifierIdentifier(WithVariable &left, WithVariable &right);

    bool isValidIdentifierAttrRef(WithVariable &left, WithVariable &right);

    bool isValidAttrRefInteger(WithVariable &left, WithVariable &right);

    bool isValidAttrRefIdentifier(WithVariable &left, WithVariable &right);

    bool isValidAttrRefAttrRef(WithVariable &left, WithVariable &right);

    bool isAttrRefValid(WithVariable &variable);

    bool isStmtNumVar(WithVariable &variable);

    bool isValueVar(WithVariable &variable);

    bool isProcVar(WithVariable &variable);

    bool isVarNameVar(WithVariable &variable);

    Table *evaluateIntegerInteger(WithVariable &left, WithVariable &right);

    Table *evaluateIntegerAttrRef(WithVariable &left, WithVariable &right);

    Table *evaluateIdentifierIdentifier(WithVariable &left, WithVariable &right);

    Table *evaluateIdentifierAttrRef(WithVariable &left, WithVariable &right);

    Table *evaluateAttrRefInteger(WithVariable &left, WithVariable &right);

    Table *evaluateAttrRefIdentifier(WithVariable &left, WithVariable &right);

    Table *evaluateAttrRefAttrRef(WithVariable &left, WithVariable &right);

    unordered_set<string> getSpecificStatementType(QueryDeclaration::design_entity_type type);

    unordered_set<string> getName(QueryDeclaration::design_entity_type type);

    Table *buildSingleSynonymTableWithSingleFilter(unordered_set<string> &results, QueryDeclaration &synonym,
                                                   const string &filter, const ValueMapping &mapping);

    Table *buildSynonymSynonymTable(unordered_set<string> &leftResults, unordered_set<string> &rightResults,
                                    QueryDeclaration &leftSynonym, QueryDeclaration &rightSynonym,
                                    const ValueMapping &leftMapping, const ValueMapping &rightMapping);

    Table *buildSameSynonymTable(unordered_set<string> &results, const string &column);

    Table *buildDiffSynonymTable(unordered_set<string> &leftResults, unordered_set<string> &rightResults,
                                 const string &firstColumn, const string &secondColumn,
                                 const ValueMapping &leftMapping, const ValueMapping &rightMapping);

    ValueMapping sameMapper = [](const string &stmtNum, PKBClient *) { return stmtNum; };

    ValueMapping getMapper(QueryDeclaration::design_entity_type type);

public:
    explicit WithEvaluator(PKBClient *pkb);

    Table* evaluate(const WithClause &clause);
};