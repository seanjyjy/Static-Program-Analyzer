#pragma once

// Represents the type of a SIMPLE Abstract Syntax Tree node.
enum class TNodeType {
  program,
  procedure,
  stmtLst,
  readStmt, // read x
  printStmt, // print x
  callStmt, // call "main"
  whileStmt, // while
  ifStmt, // if then else
  assignStmt, // =
  notOp, // !
  andOp, // &&
  orOp, // ||
  gt, // >
  ge, // >=
  lt, // <
  le, // <=
  eq, // ==
  ne, // !=
  plus, // +
  minus, // -
  times, // *
  div, // /
  mod, // %
  varName, // x
  procName, // "main"
  constValue, // 123

  dummy, // for parsing to ast
};

// TODO: refactor: this could be moved into the TNode class instead.
static bool isStatement(TNodeType type) {
  switch (type) {
	case TNodeType::readStmt:
	case TNodeType::printStmt:
	case TNodeType::callStmt:
	case TNodeType::whileStmt:
	case TNodeType::ifStmt:
	case TNodeType::assignStmt:return true;
	default:return false;
  }
}

static bool isCondExpr(TNodeType type) {
  switch (type) {
	case TNodeType::notOp:
	case TNodeType::andOp:
	case TNodeType::orOp:
	case TNodeType::gt:
	case TNodeType::ge:
	case TNodeType::lt:
	case TNodeType::le:
	case TNodeType::eq:
	case TNodeType::ne:return true;
	default:return false;
  }
}

static bool isOp(TNodeType type) {
  switch (type) {
	case TNodeType::plus:
	case TNodeType::minus:
	case TNodeType::times:
	case TNodeType::div:
	case TNodeType::mod:return true;
	default:return false;
  }
}
