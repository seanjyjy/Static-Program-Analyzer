#include <iostream>
#include "SimpleParser/Parser.h"

using namespace std;

int main() {
    string s = "procedure main {\n"
               "    read x;\n"
               "    print x;\n"
               "    read x;\n"
               "}";
    Parser p = Parser(s);
    TNode* ast = p.parse();
    if (ast != nullptr) {
        cout << ast->toStringRecursive() << endl;
    }
}