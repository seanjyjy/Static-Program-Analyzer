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

string TestAstBuilderUtils::readFile(string filename) {
    filesystem::path cwd = filesystem::current_path();
    cwd = cwd.parent_path().parent_path().parent_path();
    cwd /= "src/unit_testing/src/Common/AstBuilder/input/";
    cwd /= filename;
    string path = cwd.string();
    cout << path << endl;
    string fileContent = FileReader::getFileContent(path);
    return fileContent;
}
