#include <fstream>
#include <string>

#include "FileReader.h"

// Reference from https://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
string FileReader::getFileContent(string &path) {
    ifstream in(path, ios::in | ios::binary);

    if (in) {
        string contents;
        in.seekg(0, ios::end);
        contents.resize(in.tellg());
        in.seekg(0, ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return (contents);
    }
    throw runtime_error("Cannot open file: " + path);
}
