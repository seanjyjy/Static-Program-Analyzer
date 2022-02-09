#pragma once

#include <list>
#include <string>
#include <unordered_set>

using namespace std;

class DesignExtractorUtils {
public:
    static void combineSetsClear(unordered_set<string> &parentSet, unordered_set<string> &childSet);

    static void combineListsClear(list<string> &parentLst, list<string> &childLst);
};
