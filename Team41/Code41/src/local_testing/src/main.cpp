#include "SimpleParser/Parser.h"
#include "Common/AstBuilder.h"
#include "SimpleParser/ParserUtils.h"

using namespace std;

#include <iostream>

int main() {
    string s = "procedure main{\n"
               "a = 0941 + 10;\n"
               "b = 100 + 200;\n"
               "}\n";
    cout << ParserUtils::highlight(s, 1, 1, 1, 4) << endl;
//
//    Parser p;
//    TNode* ast = p.parseProgram(s);
//    ast->printRecursive();

//    string xml = "";
//    AstBuilder b(xml);
//    b.build();
}