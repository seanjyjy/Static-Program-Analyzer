#include "Program.h"
#include "Common/TNodeType.h"
#include "Common/AstNode/Procedure.h"

vector<TNode *> convert(const vector<Procedure *> &procedures) {
  if (procedures.empty()) throw runtime_error("procedures must not be empty");

  vector<TNode *> ret;
  ret.reserve(procedures.size());
  for (Procedure *procedure : procedures) ret.push_back(static_cast<TNode *>(procedure));
  return ret;
}

Program::Program(const vector<Procedure *> &procedures) : TNode(TNodeType::program, nullptr, convert(procedures)) {
}
