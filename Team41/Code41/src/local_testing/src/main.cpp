#include <iostream>
#include "SimpleParser/Parser.h"

using namespace std;

int main() {
    string s = "procedure main {"
               "x = 1 * 2 + 3 / 4;"
               "}";

    Parser p;
    TNode* ast = p.parseProgram(s);
    if (ast != nullptr) {
        ast->printRecursive();

    }
}