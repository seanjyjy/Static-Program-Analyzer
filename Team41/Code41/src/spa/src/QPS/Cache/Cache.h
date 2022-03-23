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

    unordered_set<string> getMapping(Mapping map, const string &key);

public:
    void registerForwardMapping(const string &key, const string &value); // register key-value pair in forward mapping

    void registerBackwardMapping(const string &key, const string &value); // register key-value pair in backward mapping

    void registerBooleanMapping(const string &key, const string &value); // register key-value pair in boolean mapping

    unordered_set<string> getForwardMapping(const string &key); // retrieves the forward mapping for certain key

    unordered_set<string> getBackwardMapping(const string &key); // retrieves the backward mapping for certain key

    bool getBooleanMapping(const string &key, const string &value); // checks if key value pair exist in boolean mapping

    vector<pair<string, string>> getAllMapping(); // retrieves all mapping

    void addAllMappingPair(pair<string, string> pair); // add a pair into the vector of mappings
};

