#include "SemanticException.h"

SemanticException::SemanticException(string message) : SPAException(move(message)) {}

string SemanticException::what() { return errorMessage; }

SemanticException::~SemanticException() = default;
