#pragma once
#include <exception>
#include <string>

using namespace std;

class SyntheticException {
private:
    string errorMessage;
public:
    SyntheticException(string errorMessage);

    string what();
};