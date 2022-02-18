#include "TestAstBuilderUtils.h"

#include <filesystem>
#include <utility>
#include <iostream>
#include "Common/AstBuilder.h"
#include "Common/FileReader.h"

using namespace std;

TNode *TestAstBuilderUtils::parseXml(string xml) {
    return AstBuilder(move(xml)).build();
}

string TestAstBuilderUtils::readFile(string folder, string filename) {
    filesystem::path cwd = filesystem::current_path();
    cwd = cwd.parent_path().parent_path().parent_path();
    vector<string> paths = {"src", "unit_testing", "simple", folder, filename};
    for (const string& p: paths) {
        cwd /= p;
    }
    string path = cwd.string();
    string fileContent = FileReader::getFileContent(path);
    return fileContent;
}
