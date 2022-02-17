#include "SemanticException.h"

SemanticException::SemanticException(string message) : errorMessage(move(message)) {}

string SemanticException::what() { return errorMessage; }
