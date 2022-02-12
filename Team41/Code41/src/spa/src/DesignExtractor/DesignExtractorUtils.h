#pragma once

#include <list>
#include <string>
#include <unordered_set>

using namespace std;

class DesignExtractorUtils {
public:
    /**
     * Takes 2 sets of strings (parentSet and childSet), and copies over all elements in childSet to parentSet.
     * Clears the childSet after copying over is done.
     *
     * @param parentSet set of strings that wil be combined into
     * @param childSet set of strings whose content is to be copied out
     */
    static void combineSetsClear(unordered_set<string> &parentSet, unordered_set<string> &childSet);

    /**
     * Takes 2 lists of string (parentLst and childLst), and appends childLst to tail of parentLst.
     * Clears childLst after it's done.
     *
     * @param parentLst list of strings that will get appended to
     * @param childLst list of string that will get appended
     */
    static void combineListsClear(list<string> &parentLst, list<string> &childLst);
};
