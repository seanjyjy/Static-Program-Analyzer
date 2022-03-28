#include "Cache.h"

void Cache::registerForwardMapping(const string &key, const string &value) {
    forwardMapping[key].insert(value);
}

void Cache::registerBackwardMapping(const string &key, const string &value) {
    backwardMapping[key].insert(value);
}

void Cache::registerBooleanMapping(const string &key, const string &value) {
    booleanMapping[key].insert(value);
}

unordered_set<string> Cache::getForwardMapping(const string &key) {
    return getMapping(forwardMapping, key);
}

unordered_set<string> Cache::getBackwardMapping(const string &key) {
    return getMapping(backwardMapping, key);
}

bool Cache::getBooleanMapping(const string &key, const string &value) {
    unordered_set<string> values = getMapping(booleanMapping, key);
    return values.find(value) != values.end();
}

vector<pair<string, string>> Cache::getAllMapping() {
    return allMapping;
}

void Cache::addAllMappingPair(const pair<string, string> &pair) {
    allMapping.push_back(pair);
}

unordered_set<string> Cache::getMapping(Mapping map, const string &key) {
    if (map.find(key) != map.end()) {
        return map[key];
    }
    return {};
}
