#include "SyntaxException.h"

using namespace std;

#include <utility>

SyntaxException::SyntaxException(string message) : SPAException(move(message)) {}

string SyntaxException::what() { return errorMessage; }

SyntaxException::~SyntaxException() = default;