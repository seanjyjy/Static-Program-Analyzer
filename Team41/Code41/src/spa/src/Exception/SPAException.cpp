#include "SPAException.h"

using namespace std;

#include <utility>

SPAException::SPAException(string errorMessage) : errorMessage(move(errorMessage)) {}
