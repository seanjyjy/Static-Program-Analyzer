#include "TokenizeException.h"

#include <utility>

using namespace std;

TokenizeException::TokenizeException(string message) : SPAException(move(message)) {}

TokenizeException::TokenizeException(): SPAException("") {}

string TokenizeException::what() { return errorMessage; }

TokenizeException::~TokenizeException() = default;
