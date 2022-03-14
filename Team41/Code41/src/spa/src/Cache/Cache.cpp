#include "Cache.h"

void Cache::registerForwardMapping(const string& key, const string& value) {
    forwardMapping[key].insert(value);
}

void Cache::registerBackwardMapping(const string& key, const string& value) {
    backwardMapping[key].insert(value);
}

void Cache::registerBooleanMapping(const string& key, const string& value) {
    booleanMapping[key].insert(value);
}

unordered_set<string> Cache::getForwardMapping(const string& key) {
    if (forwardMapping.find(key) == forwardMapping.end()) {
        return {};
    }

    return forwardMapping[key];
}

unordered_set<string> Cache::getBackwardMapping(const string& key) {
    if (backwardMapping.find(key) == forwardMapping.end()) {
        return {};
    }

    return backwardMapping[key];
}

bool Cache::getBooleanMapping(const string& key, const string& value) {
    if (booleanMapping.find(key) == booleanMapping.end()) {
        return false;
    }

    unordered_set<string> values = booleanMapping[key];
    return values.find(value) != values.end();
}

size_t Cache::getForwardMapSize() {
    return forwardMapping.size();
}

size_t Cache::getBackwardMapSize() {
    return backwardMapping.size();
}

vector<pair<string, string>> Cache::getAllMapping() {
    return allMapping;
}

void Cache::addAllMappingPair(pair<string, string> pair) {
    allMapping.push_back(pair);
}


