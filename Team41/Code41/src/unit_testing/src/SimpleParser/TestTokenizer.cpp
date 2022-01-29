#include <iostream>

#include "Common/TNode.h"
#include "SimpleParser/Tokenizer.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("Tokenizer: 1st Test") {
    // TODO: look into autotester functions, make this into a real test case
    string s = "procedure main {\n"
               "    one = 0;\n"
               "    two = 0;\n"
               "\n"
               "    read x;\n"
               "    printRecursive x;\n"
               "\n"
               "    while (!(1 > 2) && (3 >= 4)) {\n"
               "        one = 1;\n"
               "    }\n"
               "\n"
               "    if ((one != two) || (1 <= 2)) {\n"
               "        two = 2;\n"
               "    }\n"
               "}";
    Tokenizer tokenizer(s);
    vector<Token> tokens = tokenizer.tokenize();
    for (Token& t: tokens) {
        cout << t.toString() << endl;
    }
}
