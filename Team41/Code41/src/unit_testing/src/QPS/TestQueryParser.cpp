#include "catch.hpp"

#include "QPS/QueryParser.h"

using namespace std;

TEST_CASE() {
    SECTION("VALID DECLARATION") {
        string s = "variable v; assign a;\n"
                   "Select v such that Uses(a, v)";

        QueryParser qp = QueryParser{s};
        qp.parse();




        REQUIRE(true);
    }
}
