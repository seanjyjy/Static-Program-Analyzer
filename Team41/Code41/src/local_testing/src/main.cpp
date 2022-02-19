#include "SimpleParser/Parser.h"
#include "Common/AstBuilder.h"
#include "SimpleParser/ParserUtils.h"

using namespace std;

#include <iostream>

int main() {
    string s = "procedure main{\n"
               "\ta = 9341 + 10;\n"
               "b = 100 + 200;\n"
               "}\n";
    cout << ParserUtils::highlight(s, 0, 0, 0, 4) << endl;
//
//    Parser p;
//    TNode* ast = p.parseProgram(s);
//    ast->printRecursive();

//    string xml = "";
//    AstBuilder b(xml);
//    b.build();
}