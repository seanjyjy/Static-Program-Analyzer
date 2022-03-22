#include "catch.hpp"
#include "SimpleParser/Parser.h"
#include "DesignExtractor/DesignExtractor.h"
#include "QPS/QueryEvaluator.h"
#include "QPS/QueryParser.h"
#include "QPS/Optimizer/QueryOptimizer.h"

#include <string>

using namespace std;

TEST_CASE("Query Optimizer") {
    string simple = "procedure p {\n"
                    "  if (x<0) then {\n"
                    "    while (i>0) {\n"
                    "      x = z * 3 + 2 * y;\n"
                    "      call q;\n"
                    "      i = i - 1; }\n"
                    "    x = x + 1;\n"
                    "    z = x + z; }\n"
                    "  else {\n"
                    "    z = 1; }\n"
                    "  z = z + x + i; }\n"
                    "procedure q {\n"
                    "  x = 2; }";

    // parse program
    Parser p;
    unique_ptr<TNode> ast(p.parseProgram(simple));

    // extract relations into pkb
    PKBManager pkbManager = PKBManager();
    DesignExtractor designExtractor(ast.get(), &pkbManager);
    designExtractor.extractDesign();

    // answer pql
    string pql = "variable v;\n"
                 "Select v such that Uses(\"p\", v)";

    QueryParser qp = QueryParser{pql};
    unique_ptr<QueryObject> queryObject(qp.parse());
    OptimizedQueryObject oqo = QueryOptimizer::create()
            .setClauseGrouping(true)
            .setInterGroupSort(true)
            .setIntraGroupSort(true)
            .optimize(queryObject.get());
    oqo.print();
//    QueryOptimizer optimizer(&pkbManager);
//    optimizer.optimize(*queryObject);
//    optimizer.printPlan();
    // TODO complete
    delete &oqo;
}