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
    return getMapping(forwardMapping, key);
}

unordered_set<string> Cache::getBackwardMapping(const string& key) {
    return getMapping(backwardMapping, key);
}

bool Cache::getBooleanMapping(const string& key, const string& value) {
    unordered_set<string> values = getMapping(booleanMapping, key);
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

void Cache::printForwardMapping() {
    printMapping(forwardMapping);
}

void Cache::printBooleanMapping() {
    printMapping(booleanMapping);
}

void Cache::printBackwardMapping() {
    printMapping(forwardMapping);
}

void Cache::printMapping(Mapping map) {
    for (auto&[key, values] : map) {
        printf("key: %s values: ", key.c_str());
        for (auto& value : values) {
            printf("%s ", value.c_str());
        }
        printf("\n");
    }
}

unordered_set<string> Cache::getMapping(Mapping map, const string &key) {
    if (map.find(key) != map.end()) {
        return map[key];
    }
    return {};
}
