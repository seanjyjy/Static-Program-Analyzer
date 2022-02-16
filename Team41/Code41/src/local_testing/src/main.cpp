#include "SimpleParser/Parser.h"
#include "Common/AstBuilder.h"

using namespace std;

int main() {
    string s = "procedure n3iwl {\n"
               "\tprocedure = procedure + 1;\n"
               "\tif ((a * b * c == 1) && (if % else % then <= 2)) then {\n"
               "\t\tprint = 98 * 20 + 2;\n"
               "\t\twhile (((and * two) >= 1) || ( 1 <= (and * two))) {\n"
               "\t\t\tread read;\n"
               "\t\t\tif (0 == 0) then {\n"
               "\t\t\t\tprint print;\n"
               "\t\t\t\twhile (bob == builder) {\n"
               "\t\t\t\t\ttry = catch + 1;\n"
               "\t\t\t\t}\n"
               "\t\t\t} else {\n"
               "\t\t\t\tread reader;\n"
               "\t\t\t\twhile ((while + 1) == (while + 1)) {\n"
               "\t\t\t\t\twhile = while * while;\n"
               "\t\t\t\t}\n"
               "\t\t\t}\n"
               "\t\t}\n"
               "\t} else {\n"
               "\t\tprint x;\n"
               "\t}\n"
               "}";
    Parser p;
    TNode* ast = p.parseProgram(s);
//    ast->printRecursive();

//    string xml = "";
//    AstBuilder b(xml);
//    b.build();
}