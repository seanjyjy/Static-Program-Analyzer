#pragma once

#include <string>
#include <unordered_set>
#include "QPS/Entities/Readers/EntitiesReader.h"

using namespace std;

class Entities {
private:
    EntitiesReader *reader;
public:
    explicit Entities(EntitiesReader *);

    virtual ~Entities();

    virtual bool isStmt();

    virtual bool isRead();

    virtual bool isPrint();

    virtual bool isCall();

    virtual bool isWhile();

    virtual bool isIf();

    virtual bool isAssign();

    virtual bool isVariable();

    virtual bool isConstant();

    virtual bool isProcedure();

    virtual bool isNone();

    virtual string toString() = 0;

    virtual int hashCode() = 0;

    virtual EntitiesReader *getReader() = 0;

    friend class StmtEntities;

    friend class ReadEntities;

    friend class PrintEntities;

    friend class CallEntities;

    friend class WhileEntities;

    friend class IfEntities;

    friend class AssignEntities;

    friend class VariableEntities;

    friend class ConstantEntities;

    friend class ProcedureEntities;

    friend class NoneEntities;
};
