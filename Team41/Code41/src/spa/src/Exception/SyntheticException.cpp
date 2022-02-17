#include "SyntheticException.h"

SyntheticException::SyntheticException(string message) : errorMessage(move(message)) {}

string SyntheticException::what() { return errorMessage; }

