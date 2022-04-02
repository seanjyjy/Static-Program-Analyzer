#pragma once

#include "Evaluator.h"
#include <functional>

// If required is a stmt, return back the stmt provided, else get its required mapping through the function provided
typedef std::function<string(const string &, PKBClient *pkb)> ValueMapping;

class WithEvaluator : public Evaluator {
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

    Table *evaluateAttrRefAttrRef(WithVariable &left, WithVariable &right, bool canSimplify);

    unordered_set<string> getSpecificStatementType(Entities *type);

    unordered_set<string> getName(Entities *type);

    Table *buildSingleSynonymTableWithSingleFilter(unordered_set<string> &results, QueryDeclaration &synonym,
                                                   const string &filter, const ValueMapping &mapping);

    Table *buildSynonymSynonymTable(unordered_set<string> &leftRes, unordered_set<string> &rightRes,
                                    QueryDeclaration &leftSynonym, QueryDeclaration &rightSynonym,
                                    const ValueMapping &leftMapping, const ValueMapping &rightMapping,
                                    bool canSimplify);

    Table *buildSameSynonymTable(unordered_set<string> &results, const string &column);

    Table *buildDiffSynonymTable(unordered_set<string> &leftResults, unordered_set<string> &rightResults,
                                 const string &firstColumn, const string &secondColumn,
                                 const ValueMapping &leftMapping, const ValueMapping &rightMapping, bool canSimplify);

    ValueMapping sameMapper = [](const string &stmtNum, PKBClient *) { return stmtNum; };

    ValueMapping getMapper(Entities *type);

public:
    explicit WithEvaluator(PKBClient *pkb);

    /**
     * Constructs a table that contains the result based on With Clause relation
     *
     * @param clause A WithClause relation representing the clause provided in the query
     * @return A result of the clause query in a table format.
     */
    Table *evaluate(const WithClause &clause);
};