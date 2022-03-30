#include "catch.hpp"

#include <string>
#include "TestOptimizerUtils.h"

using namespace std;

TEST_CASE("Group and Groups Abstraction: simple") {
    string query = "assign a;\n"
                   "Select a pattern a(\"z\", _\"x\"_)";
    TestOptimizerUtils::testGroupandGroupsAbstractions(query);
}

TEST_CASE("Group and Groups Abstraction: medium") {
    string query = "assign a1, a2, a3; stmt s1, s2, s3; variable v1, v2, v3;\n"
                   "Select <s1, s2, s3> such that Uses(s3, v1) and Modifies(s3, \"x\") and Follows(s1, s2) and Parent(s3, s1) and Uses(s2, v1)";
    TestOptimizerUtils::testGroupandGroupsAbstractions(query);
}

TEST_CASE("Group and Groups Abstraction: complex") {
    string query = "assign a1, a2, a3; stmt s1, s2, s3; variable v1, v2, v3;\n"
                   "Select <s1, s2, s3> "
                   "such that Uses(s3, v1) "
                   "and Modifies(s3, \"x\") "
                   "and Follows(s1, s2) "
                   "and Parent(s3, s1) "
                   "and Uses(s2, v1) "
                   "such that Uses(5, \"y\") "
                   "and Follows(3, 4) "
                   "pattern a1(v2, _\"x+y\"_) "
                   "such that Affects(a1, a2) "
                   "with a2.stmt# = 20 "
                   "such that Modifies(a3, v3) "
                   "pattern a3(\"z\", _)";
    TestOptimizerUtils::testGroupandGroupsAbstractions(query);
}