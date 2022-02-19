#include "SimpleParser/Parser.h"
#include "Common/AstBuilder.h"
#include "SimpleParser/ParserUtils.h"

using namespace std;

#include <iostream>

int main() {
    string s = "procedure main {\n"
               " while (0==0) {}\n"
               "}";
//    cout << ParserUtils::highlight(s, 1, 1, 1, 4) << endl;
//
    Parser p;
    TNode* ast = p.parseProgram(s);
//    ast->printRecursive();

//    string xml = "";
//    AstBuilder b(xml);
//    b.build();
}