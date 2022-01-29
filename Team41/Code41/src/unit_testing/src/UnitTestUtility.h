#pragma once

#include <vector>
#include <algorithm>

class UnitTestUtility {
public:
    template <class T>
    static bool sortAndCompareVectors(std::vector<T> a, std::vector<T> b);
};

#include "UnitTestUtility.tpp"
