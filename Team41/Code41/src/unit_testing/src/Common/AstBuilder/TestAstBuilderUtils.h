#pragma once

#include "Common/TNode.h"
#include <string>

using namespace std;

// wrapper class to protect from changes in original source code
class TestAstBuilderUtils {
public:
    static TNode *parseXml(string xml);

    static string readFile(string folder, string filename);
};
