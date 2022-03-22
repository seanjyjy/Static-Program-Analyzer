#include "Tokens.h"

#include <vector>

#include "Token.h"

#include <stdexcept>
#include <iostream>

using namespace std;

Tokens::Tokens() = default;

void Tokens::add(Token &t) {
  tokens.push_back(t);
}

bool Tokens::empty() {
  return tokens.empty();
}

Token &Tokens::operator[](int idx) {
  if (idx < 0 || idx >= (int) tokens.size()) throw out_of_range("vector out of bounds");
  return tokens[idx];
}

size_t Tokens::size() {
  return tokens.size();
}

void Tokens::print() {
  for (const Token &t : tokens) {
	cout << t.toString() << endl;
  }
}

void Tokens::clear() {
  tokens.clear();
}


