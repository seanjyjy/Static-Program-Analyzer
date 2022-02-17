#pragma once
#include <exception>
#include <string>

using namespace std;

class SemanticException : public exception {
private:
    string errorMessage;
public:
    SemanticException(string errorMessage);

    string what();
};

