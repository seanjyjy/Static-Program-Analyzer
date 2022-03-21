#pragma once

#include <iostream>
#include <filesystem>
#include "Common/FileReader.h"
#include "Common/TreeUtils.h"
#include <list>

class TestDesignExtractorUtils {
public:
    static string readFile(string folder, string filename) {
        filesystem::path cwd = filesystem::current_path();
        cwd = cwd.parent_path().parent_path().parent_path();
        list<string> paths = {"src", "unit_testing"};
        if (!folder.empty())
            paths.push_back(folder);
        paths.push_back(filename);
        for (const string& p: paths) {
            cwd /= p;
        }
        string path = cwd.string();
        string fileContent = FileReader::getFileContent(path);
        return fileContent;
    }

    static string readDeInput(string filename) {
        return readFile("src/DesignExtractor/input", filename);
    }

    static string readDePattern(string patternFolder, string filename) {
        return readFile("src/DesignExtractor/input/pattern/" + patternFolder, filename);
    }

    static string readSimpleProgram(string filename) {
        return readFile("simple/program", filename);
    }

    static bool isPatternEqual(unordered_map<string, pair<string, TNode *>> mp1, unordered_map<string, pair<string, TNode *>> mp2) {
        if (mp1.size() != mp2.size()) return false;
        for (auto &[stmt, p1] : mp1) {
            if (mp2.find(stmt) == mp2.end()) return false;
            pair<string, TNode *> p2 = mp2.at(stmt);
            if (p1.first != p2.first || !TreeUtils::isEqual(p1.second, p2.second))
                return false;
        }
        return true;
    }
};
