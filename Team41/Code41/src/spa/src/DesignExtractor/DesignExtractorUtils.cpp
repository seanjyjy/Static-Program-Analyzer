#include "DesignExtractorUtils.h"

void DesignExtractorUtils::combineSets(unordered_set<string> &parentSet, unordered_set<string> &childSet) {
    parentSet.insert(childSet.begin(), childSet.end());
}
