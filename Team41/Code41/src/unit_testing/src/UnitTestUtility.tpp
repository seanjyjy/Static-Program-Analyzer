#include "UnitTestUtility.h"

// https://stackoverflow.com/a/40769636
template<class T>
static bool sortAndCompareVectors(std::vector<T> a, std::vector<T> b) {
    if (a.size() != b.size()) {
        return false;
    }
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return (a == b);
}