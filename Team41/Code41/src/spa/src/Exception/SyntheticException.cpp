#include "SyntheticException.h"

SyntheticException::SyntheticException(string message) : SPAException(message) {}

string SyntheticException::what() { return errorMessage; }

SyntheticException::~SyntheticException() = default;