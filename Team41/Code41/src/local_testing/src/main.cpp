#include "SimpleParser/Parser.h"
#include "Common/AstBuilder.h"

using namespace std;

int main() {
    string s = "<eq>\n"
               "\t<var val=hello></var>\n"
               "\t<const val=1></const>\n"
               "</eq>";

    TNode *root = AstBuilder(s).build();
    root->printRecursive();
}