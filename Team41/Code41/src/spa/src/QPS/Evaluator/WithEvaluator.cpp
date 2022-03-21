#include "WithEvaluator.h"

WithEvaluator::WithEvaluator(PKBClient *pkb) : Evaluator(pkb) {}

// ==================================== PUBLIC METHODS =========================================================

Table *WithEvaluator::evaluate(WithClause *clause) {
    WithVariable left = clause->getLeft();
    WithVariable right = clause->getRight();

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
        return evaluateAttrRefAttrRef(left, right);
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
    QueryDeclaration::design_entity_type type = synonym.getType();

    if (isValueVar(variable)) {
        return EvaluatorUtils::isConstant(type);
    }

    if (isProcVar(variable)) {
        return EvaluatorUtils::isProcedure(type) || EvaluatorUtils::isCall(type);
    }

    if (isVarNameVar(variable)) {
        return EvaluatorUtils::isVariable(type) || EvaluatorUtils::isRead(type) || EvaluatorUtils::isPrint(type);
    }

    return EvaluatorUtils::isStmt(type) ||  EvaluatorUtils::isRead(type) || EvaluatorUtils::isPrint(type)
        || EvaluatorUtils::isCall(type) || EvaluatorUtils::isWhile(type) || EvaluatorUtils::isIf(type)
        || EvaluatorUtils::isAssign(type);
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

Table *WithEvaluator::buildSingleSynonymTableWithSingleFilter(unordered_set<string>& results, QueryDeclaration& synonym,
                                                              const string& filter, const ValueMapping& mapping) {
    string column = synonym.getSynonym();
    Header header = Header({column});
    Table* table = new PQLTable(header);
    for (auto& result : results) {
        if (mapping(result, pkb) == filter) {
            Row* row = new Row(column, result);
            table->addRow(row);
        }
    }

    return table;
}

Table *WithEvaluator::buildSynonymSynonymTable(unordered_set<string> &leftResults, unordered_set<string>& rightResults,
                                              QueryDeclaration &leftSynonym, QueryDeclaration &rightSynonym,
                                              const ValueMapping &leftMapping, const ValueMapping &rightMapping) {
    string firstColumn = leftSynonym.getSynonym();
    string secondColumn = rightSynonym.getSynonym();

    if (firstColumn == secondColumn) {
        return buildSameSynonymTable(leftResults, firstColumn);
    }

    return buildDiffSynonymTable(leftResults, rightResults, firstColumn, secondColumn, leftMapping, rightMapping);
}

Table *WithEvaluator::buildDiffSynonymTable(unordered_set<string> &leftResults, unordered_set<string> &rightResults,
                                            const string& firstColumn, const string& secondColumn,
                                            const ValueMapping &leftMapping, const ValueMapping &rightMapping) {
    Header header = Header({firstColumn, secondColumn});
    Table* table = new PQLTable(header);

    for (auto& leftResult : leftResults) {
        for (auto& rightResult : rightResults) {
            if (leftMapping(leftResult, pkb) == rightMapping(rightResult, pkb)) {
                Row* row = new Row();
                row->addEntry(firstColumn, leftResult);
                row->addEntry(secondColumn, rightResult);
                table->addRow(row);
            }
        }
    }

    return table;
}

Table *WithEvaluator::buildSameSynonymTable(unordered_set<string> &results, const string& column) {
    Header header = Header({column});
    Table* table = new PQLTable(header);

    for (auto& result : results) {
        Row* row = new Row(column, result);
        table->addRow(row);
    }

    return table;
}



// ==================================== EVALUATOR METHODS ================================================

Table *WithEvaluator::evaluateIntegerInteger(WithVariable &left, WithVariable &right) {
    bool isSameInteger = left.getIntegerAsString() == right.getIntegerAsString();
    return buildBooleanTable(isSameInteger);
}

Table *WithEvaluator::evaluateIntegerAttrRef(WithVariable &left, WithVariable &right) {
    // 2 = read.stmt# || 10 = c.value
    QueryDeclaration synonym = right.getSynonym();
    unordered_set<string> results = getSpecificStatementType(synonym.getType());
    return buildSingleSynonymTableWithSingleFilter(results, synonym, left.getIntegerAsString(), sameMapper);
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
    return buildSingleSynonymTableWithSingleFilter(results, synonym, right.getIntegerAsString(), sameMapper);
}

Table *WithEvaluator::evaluateAttrRefIdentifier(WithVariable &left, WithVariable &right) {
    QueryDeclaration synonym = left.getSynonym();
    unordered_set<string> results = getName(synonym.getType());
    ValueMapping mapper = getMapper(synonym.getType());
    return buildSingleSynonymTableWithSingleFilter(results, synonym, right.getIdent(), mapper);
}

Table *WithEvaluator::evaluateAttrRefAttrRef(WithVariable &left, WithVariable &right) {
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
        return buildSynonymSynonymTable(leftResults, rightResults, leftSynonym, rightSynonym, sameMapper, sameMapper);
    }

    unordered_set<string> leftResults = getName(leftSynonym.getType());
    unordered_set<string> rightResults = getName(rightSynonym.getType());
    ValueMapping leftMapper = getMapper(leftSynonym.getType());
    ValueMapping rightMapper = getMapper(rightSynonym.getType());
    return buildSynonymSynonymTable(leftResults, rightResults, leftSynonym, rightSynonym, leftMapper, rightMapper);
}

unordered_set<string> WithEvaluator::getSpecificStatementType(QueryDeclaration::design_entity_type type) {
    if (EvaluatorUtils::isStmt(type)) {
        return pkb->getStatements();
    }

    if (EvaluatorUtils::isCall(type)) {
        return pkb->getCalls();
    }

    if (EvaluatorUtils::isWhile(type)) {
        return pkb->getWhiles();
    }

    if (EvaluatorUtils::isIf(type)) {
        return pkb->getIfs();
    }

    if (EvaluatorUtils::isAssign(type)) {
        return pkb->getAssigns();
    }

    if (EvaluatorUtils::isRead(type)) {
        return pkb->getReads();
    }

    if (EvaluatorUtils::isPrint(type)) {
        return pkb->getPrints();
    }

    // The above are call stmt numbers but constant itself is a number so it is included in here
    if (EvaluatorUtils::isConstant(type)) {
        return pkb->getConstants();
    }

    string errorMessage = "You shouldnt be calling getSpecificStatementType with this type: " + to_string(type);
    throw SemanticException(errorMessage);
}

unordered_set<string> WithEvaluator::getName(QueryDeclaration::design_entity_type type) {
    if (EvaluatorUtils::isProcedure(type)) {
        return pkb->getProcedures();
    }

    if (EvaluatorUtils::isVariable(type)) {
        return pkb->getVariables();
    }

    if (EvaluatorUtils::isCall(type)) {
        return pkb->getCalls();
    }

    if (EvaluatorUtils::isPrint(type)) {
        return pkb->getPrints();
    }

    if (EvaluatorUtils::isRead(type)) {
        return pkb->getReads();
    }

    string errorMessage = "You shouldnt be calling getName with this type: " + to_string(type);
    throw SemanticException(errorMessage);
}

ValueMapping WithEvaluator::getMapper(QueryDeclaration::design_entity_type type) {
    if (EvaluatorUtils::isVariable(type) || EvaluatorUtils::isProcedure(type)) {
        return sameMapper;
    }

    if (EvaluatorUtils::isCall(type)) {
        return [](const string &stmtNum, PKBClient* pkb) { return pkb->getCallsProcNameAttr(stmtNum); };
    }

    if (EvaluatorUtils::isPrint(type)) {
        return [](const string &stmtNum, PKBClient* pkb) { return pkb->getPrintVarNameAttr(stmtNum); };
    }

    if (EvaluatorUtils::isRead(type)) {
        return [](const string &stmtNum, PKBClient* pkb) { return pkb->getReadVarNameAttr(stmtNum); };
    }

    string errorMessage = "You shouldnt be calling getMapper with this type: " + to_string(type);
    throw SemanticException(errorMessage);
}