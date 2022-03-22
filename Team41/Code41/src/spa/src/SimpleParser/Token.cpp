#include "Token.h"

#include <utility>
#include <stdexcept>

Token::Token() = default;

Token::Token(TokenType type, string val, pair<int, int> start, pair<int, int> end) : type(type), val(std::move(val)),
																					 start(std::move(start)),
																					 end(std::move(end)) {}

Token::Token(TokenType type, string val) : type(type), val(move(val)), start({-1, -1}), end({-1, -1}) {}

Token *Token::makeName(string val) {
  return new Token(TokenType::name, move(val));
}

Token *Token::makeConst(string val) {
  return new Token(TokenType::integer, move(val));
}

TokenType Token::getType() const {
  return type;
}

string Token::getVal() const {
  return val;
}

pair<int, int> Token::getStart() const {
  return start;
}

pair<int, int> Token::getEnd() const {
  return end;
}

Token *Token::copy() const {
  return new Token(getType(), getVal(), getStart(), getEnd());
}

string Token::toString() const {
  return "Token{" + typeToString(type) + ", '" + val + "'" +
	  ", " + "{" + to_string(start.first) + "," + to_string(start.second) + "}" +
	  ", " + "{" + to_string(end.first) + "," + to_string(end.second) + "}" + "'}";
}

string Token::typeToString(TokenType type) {
  switch (type) {
	case TokenType::openingBrace:return "openingBrace";
	case TokenType::closingBrace:return "closingBrace";
	case TokenType::openingBracket:return "openingBracket";
	case TokenType::closingBracket:return "closingBracket";
	case TokenType::assign:return "assign";
	case TokenType::semicolon:return "semicolon";
	case TokenType::notOp:return "notOp";
	case TokenType::andOp:return "andOp";
	case TokenType::orOp:return "orOp";
	case TokenType::gt:return "gt";
	case TokenType::ge:return "ge";
	case TokenType::lt:return "lt";
	case TokenType::le:return "le";
	case TokenType::eq:return "eq";
	case TokenType::ne:return "ne";
	case TokenType::plus:return "plus";
	case TokenType::minus:return "minus";
	case TokenType::times:return "times";
	case TokenType::div:return "div";
	case TokenType::mod:return "mod";
	case TokenType::name:return "name";
	case TokenType::integer:return "integer";
	case TokenType::eof:return "eof";
	default:throw runtime_error("unknown token type");
  }
}
