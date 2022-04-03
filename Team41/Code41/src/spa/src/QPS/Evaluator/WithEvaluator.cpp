#include "WithEvaluator.h"

WithEvaluator::WithEvaluator(PKBClient *pkb) : Evaluator(pkb) {}

// ==================================== PUBLIC METHODS =========================================================

Table *WithEvaluator::evaluate(const WithClause &clause) {
    WithVariable left = clause.getLeft();
    WithVariable right = clause.getRight();

    if (isIntegerInteger(left, right)) {
        return evaluateIntegerInteger(left, right);
    }

    if (isValidIntegerAttrRef(left, right)) {
        return evaluateIntegerAttrRef(left, right);
    }

    if (isIdentifierIdentifier(left, right)) {
        return evaluateIdentifierIdentifier(left, right);
    }

    if (isValidIdentifierAttrRef(left, right)) {
        return evaluateIdentifierAttrRef(left, right);
    }

    if (isValidAttrRefInteger(left, right)) {
        return evaluateAttrRefInteger(left, right);
    }

    if (isValidAttrRefIdentifier(left, right)) {
        return evaluateAttrRefIdentifier(left, right);
    }

    if (isValidAttrRefAttrRef(left, right)) {
        return evaluateAttrRefAttrRef(left, right, clause.canSimplifyClause());
    }

    throw SemanticException("Wrong Type for With Clause");
}

// ==================================== CLAUSE CHECKER METHODS ================================================

bool WithEvaluator::isStmtNumVar(WithVariable &variable) {
    return variable.getAttr() == WithVariable::attributeName::STMT_NUM;
}

bool WithEvaluator::isValueVar(WithVariable &variable) {
    return variable.getAttr() == WithVariable::attributeName::VALUE;
}

bool WithEvaluator::isProcVar(WithVariable &variable) {
    return variable.getAttr() == WithVariable::attributeName::PROC_NAME;
}

bool WithEvaluator::isVarNameVar(WithVariable &variable) {
    return variable.getAttr() == WithVariable::attributeName::VAR_NAME;
}


bool WithEvaluator::isAttrRefValid(WithVariable &variable) {
    QueryDeclaration synonym = variable.getSynonym();
    Entities *type = synonym.getType();

    if (isValueVar(variable)) {
        return type->isConstant();
    }

    if (isProcVar(variable)) {
        return type->isProcedure() || type->isCall();
    }

    if (isVarNameVar(variable)) {
        return type->isVariable() || type->isRead() || type->isPrint();
    }

    return type->isStmt() || type->isRead() || type->isPrint() || type->isCall() || type->isWhile() || type->isIf()
           || type->isAssign();
}

bool WithEvaluator::isIntegerInteger(WithVariable &left, WithVariable &right) {
    return left.isInteger() && right.isInteger();
}

bool WithEvaluator::isValidIntegerAttrRef(WithVariable &left, WithVariable &right) {
    return left.isInteger() && right.isAttrRef() && isAttrRefValid(right)
           && (isStmtNumVar(right) || isValueVar(right)); // extra check to terminate early if needed
}

bool WithEvaluator::isIdentifierIdentifier(WithVariable &left, WithVariable &right) {
    return left.isIdentifier() && right.isIdentifier();
}

bool WithEvaluator::isValidIdentifierAttrRef(WithVariable &left, WithVariable &right) {
    return left.isIdentifier() && right.isAttrRef() && isAttrRefValid(right)
           && (isProcVar(right) || isVarNameVar(right)); // extra check to terminate early if needed
}

bool WithEvaluator::isValidAttrRefInteger(WithVariable &left, WithVariable &right) {
    return left.isAttrRef() && right.isInteger() && isAttrRefValid(left)
           && (isStmtNumVar(left) || isValueVar(left)); // extra check to terminate early if needed
}

bool WithEvaluator::isValidAttrRefIdentifier(WithVariable &left, WithVariable &right) {
    return left.isAttrRef() && right.isIdentifier() && isAttrRefValid(left)
           && (isProcVar(left) || isVarNameVar(left)); // extra check to terminate early if needed
}

bool WithEvaluator::isValidAttrRefAttrRef(WithVariable &left, WithVariable &right) {
    return left.isAttrRef() && right.isAttrRef() && isAttrRefValid(left) && isAttrRefValid(right);
}

// ==================================== TABLE BUILDERS ==================================================

Table *WithEvaluator::buildSingleSynonymTableWithSingleFilter(unordered_set<string> &results, QueryDeclaration &synonym,
                                                              const string &filter, const ValueMapping &mapping) {
    string column = synonym.getSynonym();
    Header header = Header({column});
    Table *table = new PQLTable(header);
    for (auto &result: results) {
        if (mapping(result, pkb) == filter) {
            Row *row = new Row(column, result);
            table->addRow(row);
        }
    }

    return table;
}

Table *WithEvaluator::buildSynonymSynonymTable(unordered_set<string> &leftRes, unordered_set<string> &rightRes,
                                               QueryDeclaration &leftSynonym, QueryDeclaration &rightSynonym,
                                               const ValueMapping &leftMapping, const ValueMapping &rightMapping,
                                               bool canSimplify) {

    string firstCol = leftSynonym.getSynonym();
    string secondCol = rightSynonym.getSynonym();

    if (firstCol == secondCol) {
        if (canSimplify) {
            return buildBooleanTable(!leftRes.empty());
        }

        return buildSameSynonymTable(leftRes, firstCol);
    }

    return buildDiffSynonymTable(leftRes, rightRes, firstCol, secondCol, leftMapping, rightMapping, canSimplify);
}

Table *WithEvaluator::buildDiffSynonymTable(unordered_set<string> &leftResults, unordered_set<string> &rightResults,
                                            const string &firstColumn, const string &secondColumn,
                                            const ValueMapping &leftMapping, const ValueMapping &rightMapping,
                                            bool canSimplify) {

    Header header = Header({firstColumn, secondColumn});
    Table *table = new PQLTable(header);

    for (auto &leftResult: leftResults) {
        for (auto &rightResult: rightResults) {
            if (leftMapping(leftResult, pkb) != rightMapping(rightResult, pkb)) {
                continue;
            }
            if (canSimplify) {
                delete table;
                return buildBooleanTable(true);
            }

            Row *row = new Row();
            row->addEntry(firstColumn, leftResult);
            row->addEntry(secondColumn, rightResult);
            table->addRow(row);
        }
    }

    return table;
}

Table *WithEvaluator::buildSameSynonymTable(unordered_set<string> &results, const string &column) {
    Header header = Header({column});
    Table *table = new PQLTable(header);

    for (auto &result: results) {
        Row *row = new Row(column, result);
        table->addRow(row);
    }

    return table;
}



// ==================================== EVALUATOR METHODS ================================================

Table *WithEvaluator::evaluateIntegerInteger(WithVariable &left, WithVariable &right) {
    bool isSameInteger = left.getInteger() == right.getInteger();
    return buildBooleanTable(isSameInteger);
}

Table *WithEvaluator::evaluateIntegerAttrRef(WithVariable &left, WithVariable &right) {
    // 2 = read.stmt# || 10 = c.value
    QueryDeclaration synonym = right.getSynonym();
    unordered_set<string> results = getSpecificStatementType(synonym.getType());
    return buildSingleSynonymTableWithSingleFilter(results, synonym, left.getInteger(), sameMapper);
}

Table *WithEvaluator::evaluateIdentifierIdentifier(WithVariable &left, WithVariable &right) {
    bool isSameIdentifier = left.getIdent() == right.getIdent();
    return buildBooleanTable(isSameIdentifier);
}

Table *WithEvaluator::evaluateIdentifierAttrRef(WithVariable &left, WithVariable &right) {
    // "x" = q.procName || "x" == q.varName
    QueryDeclaration synonym = right.getSynonym();
    unordered_set<string> results = getName(synonym.getType());
    ValueMapping mapper = getMapper(synonym.getType());
    return buildSingleSynonymTableWithSingleFilter(results, synonym, left.getIdent(), mapper);
}

Table *WithEvaluator::evaluateAttrRefInteger(WithVariable &left, WithVariable &right) {
    QueryDeclaration synonym = left.getSynonym();
    unordered_set<string> results = getSpecificStatementType(synonym.getType());
    return buildSingleSynonymTableWithSingleFilter(results, synonym, right.getInteger(), sameMapper);
}

Table *WithEvaluator::evaluateAttrRefIdentifier(WithVariable &left, WithVariable &right) {
    QueryDeclaration synonym = left.getSynonym();
    unordered_set<string> results = getName(synonym.getType());
    ValueMapping mapper = getMapper(synonym.getType());
    return buildSingleSynonymTableWithSingleFilter(results, synonym, right.getIdent(), mapper);
}

Table *WithEvaluator::evaluateAttrRefAttrRef(WithVariable &left, WithVariable &right, bool canSimplify) {
    bool isLeftNumberType = (isStmtNumVar(left) || isValueVar(left));
    bool isRightNumberType = (isStmtNumVar(right) || isValueVar(right));

    if (isLeftNumberType ^ isRightNumberType) {
        return new FalseTable(); // should be an empty table as number shouldnt match strings like proc or var name
    }

    QueryDeclaration leftSynonym = left.getSynonym();
    QueryDeclaration rightSynonym = right.getSynonym();

    if (isLeftNumberType && isRightNumberType) {
        unordered_set<string> leftResults = getSpecificStatementType(leftSynonym.getType());
        unordered_set<string> rightResults = getSpecificStatementType(rightSynonym.getType());
        return buildSynonymSynonymTable(leftResults, rightResults, leftSynonym, rightSynonym, sameMapper, sameMapper,
                                        canSimplify);
    }

    unordered_set<string> leftResults = getName(leftSynonym.getType());
    unordered_set<string> rightResults = getName(rightSynonym.getType());
    ValueMapping leftMapper = getMapper(leftSynonym.getType());
    ValueMapping rightMapper = getMapper(rightSynonym.getType());
    return buildSynonymSynonymTable(leftResults, rightResults, leftSynonym, rightSynonym, leftMapper, rightMapper,
                                    canSimplify);
}

unordered_set<string> WithEvaluator::getSpecificStatementType(Entities *type) {
    if (type == nullptr || type->isProcedure() || type->isVariable()) {
        throw SemanticException("Wrong type passed into with");
    }

    EntitiesReader *reader = type->getReader();
    return reader->getEntities(pkb);
}

unordered_set<string> WithEvaluator::getName(Entities *type) {
    if (type == nullptr || type->isAssign() || type->isConstant() || type->isIf() || type->isStmt() || type->isWhile()) {
        throw SemanticException("Wrong type passed into with");
    }

    EntitiesReader *reader = type->getReader();
    return reader->getEntities(pkb);
}

ValueMapping WithEvaluator::getMapper(Entities *type) {
    if (type->isVariable() || type->isProcedure()) {
        return sameMapper;
    }

    if (type->isCall()) {
        return [](const string &stmtNum, PKBClient *pkbCli) { return pkbCli->getCallsProcNameAttr(stmtNum); };
    }

    if (type->isPrint()) {
        return [](const string &stmtNum, PKBClient *pkbCli) { return pkbCli->getPrintVarNameAttr(stmtNum); };
    }

    if (type->isRead()) {
        return [](const string &stmtNum, PKBClient *pkbCli) { return pkbCli->getReadVarNameAttr(stmtNum); };
    }

    throw SemanticException("Wrong type passed into with");
}