#pragma once

#include <iostream>
#include <filesystem>
#include "Common/FileReader.h"
#include "Common/TreeUtils.h"

class TestDesignExtractorUtils {
public:
    static string readFile(string folder, string filename) {
        filesystem::path cwd = filesystem::current_path();
        cwd = cwd.parent_path().parent_path().parent_path();
        list<string> paths = {"src", "unit_testing", "src", "DesignExtractor", "input"};
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

    static bool isPatternEqual(unordered_map<string, pair<string, TNode *>> mp1, unordered_map<string, pair<string, TNode *>> mp2) {
        if (mp1.size() != mp2.size()) return false;
        auto it1 = mp1.begin(), it2 = mp2.begin();
        while (it1 != mp1.end()) {
            auto &[stmt1, p1] = *it1;
            auto &[stmt2, p2] = *it2;
            //std::cout << stmt1 << " " << stmt2 << endl << p1.first << " " << p2.first << endl << " " << p1.second << " " << p2.second << endl;
            if (stmt1 != stmt2 || p1.first != p2.first || !TreeUtils::isEqual(p1.second, p2.second))
                return false;
            ++it1; ++it2;
        }
        return true;
    }
};
