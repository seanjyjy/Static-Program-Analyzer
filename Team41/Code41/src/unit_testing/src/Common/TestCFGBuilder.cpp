#include "catch.hpp"

#include "Common/CFGBuilder.h"
#include "Common/CFGUtils.h"

TEST_CASE("CFGBuilder") {
    unordered_map<string, vector<string>> stmtNextMap = {
            {"1", {"2"}}, {"2", {"3"}}, {"3", {"4", "7"}}, {"4", {"5"}},
            {"5", {"6"}}, {"6", {"3"}}, {"7", {"8", "9"}}, {"8", {"10"}},
            {"9", {"10"}}, {"10", {"11"}}, {"11", {"12"}}, {"12", {}},
            {"13", {}}
    };
    vector<string> firstStmts = {"1", "13"};
    CFGBuilder cfgBuilder = CFGBuilder(stmtNextMap, firstStmts);
    cfgBuilder.build();
    CFGNode *cfgBuilt = cfgBuilder.getCFG();

    vector<CFGNode *> cfgNodes;
    for (int i = 0; i <= 13; ++i)
        cfgNodes.push_back(new CFGNode(to_string(i)));
    cfgNodes[0]->addChild(cfgNodes[1]); cfgNodes[0]->addChild(cfgNodes[13]);

    cfgNodes[1]->addChild(cfgNodes[2]); cfgNodes[2]->addParent(cfgNodes[1]);
    cfgNodes[2]->addChild(cfgNodes[3]); cfgNodes[3]->addParent(cfgNodes[2]);
    cfgNodes[3]->addChild(cfgNodes[4]); cfgNodes[4]->addParent(cfgNodes[3]);
    cfgNodes[3]->addChild(cfgNodes[7]); cfgNodes[7]->addParent(cfgNodes[3]);
    cfgNodes[4]->addChild(cfgNodes[5]); cfgNodes[5]->addParent(cfgNodes[4]);
    cfgNodes[5]->addChild(cfgNodes[6]); cfgNodes[6]->addParent(cfgNodes[5]);
    cfgNodes[6]->addChild(cfgNodes[3]); cfgNodes[3]->addParent(cfgNodes[6]);
    cfgNodes[7]->addChild(cfgNodes[8]); cfgNodes[8]->addParent(cfgNodes[7]);
    cfgNodes[7]->addChild(cfgNodes[9]); cfgNodes[9]->addParent(cfgNodes[7]);
    cfgNodes[8]->addChild(cfgNodes[10]); cfgNodes[10]->addParent(cfgNodes[8]);
    cfgNodes[9]->addChild(cfgNodes[10]); cfgNodes[10]->addParent(cfgNodes[9]);
    cfgNodes[10]->addChild(cfgNodes[11]); cfgNodes[11]->addParent(cfgNodes[10]);
    cfgNodes[11]->addChild(cfgNodes[12]); cfgNodes[12]->addParent(cfgNodes[11]);

    REQUIRE(CFGUtils::isEqual(cfgBuilt, cfgNodes[0]));
    // delete cfgBuilt;
    // delete cfgNodes[0];
}
