#include "Program.h"
#include "TNodeType.h"
#include "Common/AstNode/Procedure.h"

vector<TNode *> convert(const vector<Procedure *> &procedures) {
    vector<TNode *> ret;
    ret.reserve(procedures.size());
    for (Procedure *procedure: procedures) ret.push_back(static_cast<TNode *>(procedure));
    return ret;
}

Program::Program(const vector<Procedure *> &procedures) : TNode(TNodeType::program, nullptr, convert(procedures)) {
}

string Program::toString() {
    return "program";
}

bool Program::isProgram() const {
    return true;
}
