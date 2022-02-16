#include "SimpleParser/Parser.h"
#include "Common/AstBuilder.h"

using namespace std;

int main() {
    string s = "procedure n3iif {\n"
               "\tif (( ( (!(if * else >= if)) > 1 ) || (!(call >= read)) ) || (print == while)) then {\n"
               "\t\tif = 10 * if;\n"
               "\t\tif = if * if * if;\n"
               "\t} else {\n"
               "\t\tif (2 == 2) then {\n"
               "\t\t\tif (!(3 == 3)) then {\n"
               "\t\t\t\tif (100 == abe) then {\n"
               "\t\t\t\t\tprint x;\n"
               "\t\t\t\t} else {\n"
               "\t\t\t\t\tprint y;\n"
               "\t\t\t\t}\n"
               "\t\t\t\tread = read * 20;\n"
               "\t\t\t} else {\n"
               "\t\t\t\tprint = print * 30;\n"
               "\t\t\t}\n"
               "\t\t\tif = if * then % else;\n"
               "\t\t\tread if;\n"
               "\t\t} else {\n"
               "\t\t\tif (4 == 4) then {\n"
               "\t\t\t\telse = then % 4;\n"
               "\t\t\t} else {\n"
               "\t\t\t\tthen = else * 3;\n"
               "\t\t\t}\n"
               "\t\t\tcream = 10 * dollars;\n"
               "\t\t}\n"
               "\t}\n"
               "\tif = 10 + 0;\n"
               "    if = 0;\n"
               "}";
    Parser p;
    TNode* ast = p.parseProgram(s);
//    ast->printRecursive();

//    string xml = "";
//    AstBuilder b(xml);
//    b.build();
}