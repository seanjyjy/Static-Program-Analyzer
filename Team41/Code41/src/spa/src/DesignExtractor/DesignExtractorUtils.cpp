#include "DesignExtractorUtils.h"

void DesignExtractorUtils::copyOverSet(unordered_set<string> &parentSet, unordered_set<string> &childSet) {
    parentSet.insert(childSet.begin(), childSet.end());
}

void DesignExtractorUtils::combineSetsClear(unordered_set<string> &parentSet, unordered_set<string> &childSet) {
    parentSet.insert(childSet.begin(), childSet.end());
    childSet.clear();
}

void DesignExtractorUtils::combineListsClear(list<string> &parentLst, list<string> &childLst) {
    parentLst.splice(parentLst.end(), childLst);
    childLst.clear();
}
