#include "StringUtils.h"

// https://stackoverflow.com/questions/25726531/how-to-remove-leading-zeros-from-string-using-c/31226728
string StringUtils::stripLeadingZeroes(string &s) {
    return s.erase(0, min(s.find_first_not_of('0'), s.size()-1));
}
