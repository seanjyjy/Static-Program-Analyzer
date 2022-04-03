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

    virtual string toString() = 0;

    virtual int hashCode() = 0;

    bool equals(Entities* other);

    EntitiesReader *getReader();

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
};
