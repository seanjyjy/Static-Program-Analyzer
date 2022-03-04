#include "ParseException.h"

#include <utility>

using namespace std;

ParseException::ParseException(string message) : SPAException(move(message)) {}

ParseException::ParseException(): SPAException("") {}

string ParseException::what() { return errorMessage; }

ParseException::~ParseException() = default;
