#pragma once

#include "catch.hpp"
#include <unordered_map>
#include <filesystem>
#include "Common/FileReader.h"
#include "Common/TreeUtils.h"
#include "QPS/Table/Table.h"
#include <list>

class TableTestUtils {
public:
    static bool checkTableMatches(Table *table, vector<string> headers, vector<vector<string>> rows) {
        sort(rows.begin(), rows.end(),
             [](const vector<string> &a, const vector<string> &b) {
                 return a < b;
             });
        table->sort(headers);
        if (table->getHeader() != unordered_set<string>{headers.begin(), headers.end()}) {
            return false;
        }
        vector<unordered_map<string, string>> actualRows;
        for (auto row: table->getRows()) {
            actualRows.push_back(row->getRow());
        }
        vector<unordered_map<string, string>> expectedRows;
        for (vector<string> r: rows) {
            unordered_map<string, string> curr;
            for (size_t i = 0; i < headers.size(); ++i) {
                curr[headers.at(i)] = r.at(i);
            }
            expectedRows.push_back(curr);
        }

        // remove duplicate rows
        actualRows.erase(unique(actualRows.begin(), actualRows.end()), actualRows.end());

        return actualRows == expectedRows;
    }

    static void
    registerUsesModify(PKBManager *client, string proc, string stmtNum, vector<string> lhs, vector<string> rhs) {
        for (string var: lhs) {
            client->registerModifiesS(stmtNum, var);
            client->registerModifiesP(proc, var);
        }
        for (string var: rhs) {
            client->registerUsesS(stmtNum, var);
            client->registerUsesP(proc, var);
        }
    }

    static void registerCalls(PKBManager *client, string stmtNum, string proc) {
        client->registerCallStmt(stmtNum, proc);
        for (string var: client->getUsesByProc(proc)) {
            client->registerUsesS(stmtNum, var);
        }
        for (string var: client->getModifiesByProc(proc)) {
            client->registerModifiesS(stmtNum, var);
        }
    }
};
