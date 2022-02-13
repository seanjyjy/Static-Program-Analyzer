#include "SimpleParser/Parser.h"
#include "Common/AstBuilder.h"

using namespace std;

int main() {
    string s = "read x;\n"
               "print x;\n"
               "call main;\n"
               "x = 0;\n"
               "while (1==1) { print x; }\n"
               "if (1==1) then { print x; } else { print y; }"
               "}";
    Parser p;

}