#include "SimpleParser/Parser.h"
#include "Common/AstBuilder.h"
#include "SimpleParser/SPUtils.h"
#include "DesignExtractor/EntitiesExtractor.h"
#include "DesignExtractor/CallsExtractor.h"
#include "DesignExtractor/ModifiesExtractor.h"
#include <Exception/SemanticException.h>

using namespace std;

#include <iostream>

int main() {
    string s = "procedure ceeS33see {\n"
               "    if (LONG11ass22VARIABLE33in44PROCEDURE55three == GGDoTc0m) then {\n"
               "        read a0a;\n"
               "        print GGDoTc0m;\n"
               "    } else {\n"
               "        read GGDoTc0m;\n"
               "        while ((1*2-3>=4%5/Dvar1)||(((!(6==(7+8)))&&(9<=10)))) {\n"
               "            print print;\n"
               "        }\n"
               "        while ((11*12-13>=14%15/16)||(((!(17==(18+19)))&&(20<=21)))) {\n"
               "            print a0a;\n"
               "        }\n"
               "        read GGDoTc0m;\n"
               "        print GGDoTc0m;\n"
               "        call deeDEEd330987654321;\n"
               "    }\n"
               "}";

    Parser p;
    TNode *ast = p.parseProgram(s);

}