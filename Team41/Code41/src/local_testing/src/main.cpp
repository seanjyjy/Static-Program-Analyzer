#include <iostream>
#include "SimpleParser/Parser.h"

using namespace std;

int main() {
    string s = "procedure main {"
               "x = 1 * 2 + 3 / 4;"
               "}";

    Parser p = Parser{s};
    TNode* ast = p.parse();
    if (ast != nullptr) {
        ast->printRecursive();

    }
}