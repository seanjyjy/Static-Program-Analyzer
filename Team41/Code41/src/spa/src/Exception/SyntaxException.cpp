#include "SyntaxException.h"

using namespace std;

#include <utility>


SyntaxException::SyntaxException(string message) : SPAException(move(message)) {}

SyntaxException::SyntaxException(): SPAException("") {}

string SyntaxException::what() { return errorMessage; }

SyntaxException::~SyntaxException() = default;