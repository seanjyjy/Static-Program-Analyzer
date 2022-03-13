#include "Cache.h"

void Cache::registerForwardMapping(const string& key, const string& value) {
    this->forwardMapping[key].insert(value);
}

void Cache::registerBackwardMapping(const string& key, const string& value) {
    this->backwardMapping[key].insert(value);
}

void Cache::registerBooleanMapping(const string& key, const string& value) {
    this->booleanMapping[key].insert(value);
}

unordered_set<string> Cache::getForwardMapping(const string& key) {
    if (this->forwardMapping.find(key) == this->forwardMapping.end()) {
        return {};
    }

    return this->forwardMapping[key];
}

unordered_set<string> Cache::getBackwardMapping(const string& key) {
    if (this->backwardMapping.find(key) == this->forwardMapping.end()) {
        return {};
    }

    return this->backwardMapping[key];
}

bool Cache::getBooleanMapping(const string& key, const string& value) {
    if (this->booleanMapping.find(key) == this->booleanMapping.end()) {
        return false;
    }

    unordered_set<string> values = this->booleanMapping[key];
    return values.find(value) != values.end();
}


