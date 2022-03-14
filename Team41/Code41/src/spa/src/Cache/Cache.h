#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

using namespace std;

using Mapping = unordered_map<string, unordered_set<string>>;

class Cache {
private:
    Mapping forwardMapping;
    Mapping backwardMapping;
    Mapping booleanMapping;

    vector<pair<string, string>> allMapping; // only populated when getAll* is being called
public:

    void registerForwardMapping(const string& key, const string& value);

    void registerBackwardMapping(const string& key, const string& value);

    void registerBooleanMapping(const string& key, const string& value);

    unordered_set<string> getForwardMapping(const string& key);

    unordered_set<string> getBackwardMapping(const string& key);

    bool getBooleanMapping(const string& key, const string& value);

    vector<pair<string, string>> getAllMapping();

    void addAllMappingPair(pair<string, string> pair);

    size_t getForwardMapSize();

    size_t getBackwardMapSize();
};

