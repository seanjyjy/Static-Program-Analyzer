#pragma once

#include <iostream>
#include <filesystem>
#include <list>

#include "Common/FileReader.h"
#include "Common/TreeUtils.h"
#include "Common/TNodeType.h"

class TestDesignExtractorUtils {
public:
    static string readFile(string folder, string filename) {
        filesystem::path cwd = filesystem::current_path();
        cwd = cwd.parent_path().parent_path().parent_path();
        list<string> paths = {"src", "unit_testing"};
        if (!folder.empty())
            paths.push_back(folder);
        paths.push_back(filename);
        for (const string &p: paths) {
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

    static bool
    isPatternEqual(unordered_map<string, pair<string, TNode *>> mp1, unordered_map<string, pair<string, TNode *>> mp2) {
        if (mp1.size() != mp2.size()) return false;
        for (auto &[stmt, p1]: mp1) {
            if (mp2.find(stmt) == mp2.end()) return false;
            pair<string, TNode *> p2 = mp2.at(stmt);
            if (p1.first != p2.first || !TreeUtils::isEqual(p1.second, p2.second))
                return false;
        }
        return true;
    }

    static unordered_map<TNode *, string> makeNodeToStmtNumMap(TNode *ast) {
        unordered_map<TNode *, string> mp;
        int stmtNum = 0;
        stack<TNode *> stk;
        stk.push(ast);
        while (!stk.empty()) {
            TNode *node = stk.top();
            stk.pop();
            if (isStatement(node->getType()))
                mp.insert({node, to_string(++stmtNum)});
            vector<TNode *> ch = node->getChildren();
            reverse(ch.begin(), ch.end()); // left to right dfs
            for (TNode *child: ch)
                stk.push(child);
        }
        return mp;
    }

    static bool checkCallsOrder(list<string> procCallOrder, unordered_map<string, unordered_set<string>> callsMap) {
        unordered_set<string> vis;
        for (string &proc: procCallOrder) {
            if (callsMap.find(proc) == callsMap.end()) return false;
            for (const string &procChild: callsMap[proc]) {
                if (callsMap.find(procChild) != callsMap.end() && vis.find(procChild) == vis.end())
                    return false;
            }
            vis.insert(proc);
        }
        return true;
    }
};
