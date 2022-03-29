#include "SimpleParser/Parser.h"
#include "SimpleParser/SPUtils.h"
#include <Exception/SemanticException.h>
#include "DesignExtractor/DesignExtractor.h"
#include "QPS/QueryParser.h"
#include "QPS/Optimizer/QueryOptimizer.h"

using namespace std;

#include <iostream>

int main() {
//    string simple = "procedure p {\n"
//                    "  if (x<0) then {\n"
//                    "    while (i>0) {\n"
//                    "      x = z * 3 + 2 * y;\n"
//                    "      call q;\n"
//                    "      i = i - 1; }\n"
//                    "    x = x + 1;\n"
//                    "    z = x + z; }\n"
//                    "  else {\n"
//                    "    z = 1; }\n"
//                    "  z = z + x + i; }\n"
//                    "procedure q {\n"
//                    "  x = 2; }";
//
//    // parse program
//    Parser p;
//    TNode *ast(p.parseProgram(simple));
//
//    // extract relations into pkb
//    PKBManager pkbManager = PKBManager();
//    DesignExtractor designExtractor(ast, &pkbManager);
//    designExtractor.extractDesign();

    string pql = "assign a, a1;\n"
                   "Select a pattern a (\"x\", _) and pattern a1 (_, _\"x\"_)";

    // answer pql
//    string pql = "variable v;\n"
//                 "Select v such that Uses(\"p\", v)";

    QueryParser qp = QueryParser{pql};
    QueryObject *queryObject(qp.parse());
    queryObject->print();
//    OptimizedQueryObject oqo = QueryOptimizer::create()
//            .setClauseGrouping(true)
//            .setInterGroupSort(true)
//            .setIntraGroupSort(true)
//            .enableDynamicPolling(&pkbManager)
//            .optimize(queryObject);
//    oqo.print();
//
//    int count = 0;
//    while (!oqo.empty()) {
//        oqo.popClause();
//        count++;
//    }
//    cout << count << endl;
}