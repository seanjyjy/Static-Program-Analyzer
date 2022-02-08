#pragma once

#include <string>
#include <unordered_set>

using namespace std;

class DesignExtractorUtils {
public:
    static void combineSets(unordered_set<string> &parentSet, unordered_set<string> &childSet);
};
